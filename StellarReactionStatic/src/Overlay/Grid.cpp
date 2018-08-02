#include "Grid.hpp"
#include "Picture.hpp"
#include "ShipModule.hpp"
#include "Globals.hpp"
#include "BlueprintLoader.hpp"
#include "Convert.hpp"




namespace leon
{

	Grid::GridElement::GridElement(const String& texName, const Vec2& gridSlot, const Vec2& gridSize)
	{
		QuadComponentData damageIndicatorData;
		damageIndicatorData.dimensions.x = gridSize.x;
		damageIndicatorData.dimensions.y = gridSize.y;
		damageIndicatorData.permanentRot = 0;
		damageIndicatorData.center.x = 0;
		damageIndicatorData.center.y = 0;
		damageIndicatorData.texName = "effects/module_hit";
		damageIndicatorData.layer = GraphicsLayer::OverlayTop;
		damageIndicatorData.setCenterTopLeft();
		damageIndicator.reset(new QuadComponent(damageIndicatorData));

		gridPos = gridSlot;

		QuadComponentData data;
		data.layer = GraphicsLayer::OverlayMiddle;
		data.texName = texName;
		data.dimensions = static_cast<sf::Vector2f>(gridSize);
		data.setCenterTopLeft();
		module.reset(new QuadComponent(data));
	}
	void Grid::GridElement::markBroken()
	{
		module->setColor(sf::Color::Red);
	}
	void Grid::GridElement::flashDamage(float percentHealthRemain, HealthState state)
	{
		if(state == HealthState::CriticallyDamaged)
		{
			markBroken();
		}
		const float flashDur = 1.2f;
		damageIndicator->getAnimator().setAnimation("Hit", flashDur);
	}
	void Grid::GridElement::setGuiPosition(sf::Vector2f screenPos)
	{
		module->setGuiPosition(screenPos);
		damageIndicator->setGuiPosition(screenPos);
	}



	Grid::Grid(const GridData& data)
	{
		m_gridOffset.x = (data.sizeOfGrid.x - 1) / 2;//sets total size of grid
		m_gridOffset.y = (data.sizeOfGrid.y - 1) / 2;

		m_gridSize = data.gridSize;

		{
			auto tempQuadData = data.background;
			tempQuadData.dimensions = sf::Vector2f(m_gridSize.x * data.sizeOfGrid.x, m_gridSize.y * data.sizeOfGrid.y);
			tempQuadData.setCenterTopLeft();
			tempQuadData.color = sf::Color(255, 255, 255, 30); m_background.reset(new QuadComponent(tempQuadData));
		}
	}
	Grid::~Grid()
	{

	}
	void Grid::addHUDModule(const String& title, Vec2 shipModulePos)
	{
		sptr<ShipModuleData> moduleData;
		moduleData.reset(dynamic_cast<ShipModuleData*>(getGame()->getUniverse().getBlueprints().getModuleSPtr(title)->clone()));
		if(moduleData != nullptr)
		{
			dout << "\n " << title << "\t" << shipModulePos;

			m_modules.push_back(GridElement(moduleData->baseDecor.texName, shipModulePos, m_gridSize));
			auto pos = getPixelOffset(shipModulePos);
			m_modules.back().setGuiPosition(pos);
		}
	}
	void Grid::damageModule(Vec2 pos, HealthState state, float healthPercent, bool flashDamage)
	{
		for each (auto element in m_modules)
		{
			if(element.gridPos == pos)
			{
				auto color = Health::getColor(healthPercent);
				if(healthPercent < 1.f)//otherwise we set the module to yellow when we shouldn't
				{
					element.module->setColor(color);
					element.damageIndicator->setColor(color);
				}

				if(state == HealthState::CriticallyDamaged || state == HealthState::Broken)
					element.markBroken();

				if(flashDamage)
					element.flashDamage(1, state);//TODO flash damage percent
			}
		}
	}
	void Grid::reset(const List<std::pair<String, sf::Vector2i> >& modules)
	{
		clear();

		for(auto it = modules.cbegin(); it != modules.cend(); ++it)
		{
			addHUDModule(it->first, Vec2(it->second));
		}
	}
	Vec2 Grid::getScreenPosition() const
	{
		return Vec2(m_background->getGuiPosition());
	}
	sf::Vector2f Grid::getPixelOffset(Vec2 gridPosition)
	{
		gridPosition.y = -gridPosition.y;
		Vec2 gridPixelPosition = (gridPosition + m_gridOffset) * m_gridSize;
		return static_cast<sf::Vector2f>(gridPixelPosition)+m_background->getGuiPosition();
	}
	void Grid::clear()
	{
		m_modules.clear();
	}
}
