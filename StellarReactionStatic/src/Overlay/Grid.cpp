#include "Grid.hpp"
#include "Picture.hpp"
#include "ShipModule.hpp"
#include "Globals.hpp"
#include "BlueprintLoader.hpp"
#include "Convert.hpp"




namespace leon
{
	Grid::Grid(const GridData& data)
	{
		m_gridOffset.x = (data.sizeOfGrid.x - 1) / 2;
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
		moduleData.reset(dynamic_cast<ShipModuleData*>(game.getUniverse().getBlueprints().getModuleSPtr(title)->clone()));
		if(moduleData != NULL)
		{
			dout << "\n " << title << "\t" << shipModulePos;

			m_modules.push_back(GridElement(moduleData->baseDecor.texName, shipModulePos, m_gridSize));
			m_modules.back().module->setGuiPosition(getPixelOffset(shipModulePos));

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
