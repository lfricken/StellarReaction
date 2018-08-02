#include "DraggableSurface.hpp"
#include "Draggable.hpp"
#include "ShipModule.hpp"
#include "BlueprintLoader.hpp"
#include "Network.hpp"
#include "Player.hpp"
#include "Resources.hpp"
#include "Controller.hpp"
#include "Chunk.hpp"
#include "Overlay.hpp"

using namespace leon;

DraggableSurface::DraggableSurface(tgui::Gui& container, const DraggableSurfaceData& rData) : Panel(container, rData)
{
	f_initialize(rData);
}
DraggableSurface::DraggableSurface(tgui::Container& container, const DraggableSurfaceData& rData) : Panel(container, rData)
{
	f_initialize(rData);
}
DraggableSurface::~DraggableSurface()
{

}
void DraggableSurface::f_initialize(const DraggableSurfaceData& rData)
{
	m_targetShip = -1;
	m_gridOffset = rData.gridOffset;
}
void DraggableSurface::setCountedCoordinates(const List<sf::Vector2i>& rCoords)//which coordinates should we return for getElementPositions
{
	m_validGridCoords = rCoords;
}
void DraggableSurface::addDraggable(const DraggableData& rData)
{
	DraggableData copy = rData;
	copy.gridSize = m_gridSize;
	copy.myPanelOffset = m_pPanel->getAbsolutePosition();
	copy.parentPtr = this;
	sptr<WidgetBase> spDrag(new Draggable(*this->getPanelPtr(), copy));
	this->add(spDrag);
}
//List<std::pair<String, sf::Vector2f> > DraggableSurface::getValidPositions() const
//{
//	//List<std::pair<String, sf::Vector2f> > pairing;
//	//for(auto it = m_widgetList.begin(); it != m_widgetList.end(); ++it)
//	//{
//	//	Draggable* pDrag = dynamic_cast<Draggable*>((*it).get());
//	//	if(std::find(m_validCoords.begin(), m_validCoords.end(), pDrag->getPosition()) != m_validCoords.end())//if it contains
//	//		pairing.push_back(pair<String, sf::Vector2f>(pDrag->getMetaData(), pDrag->getPosition()));
//	//}
//	//return pairing;
//}
List<Pair<String, sf::Vector2i> > DraggableSurface::getElementGridPositions() const
{
	List<Pair<String, sf::Vector2i> > pairing;
	for(auto it = m_widgetList.begin(); it != m_widgetList.end(); ++it)
	{
		Draggable* pDrag = dynamic_cast<Draggable*>((*it).get());
		pairing.push_back(Pair<String, sf::Vector2i>(pDrag->getMetaData(), pDrag->getGridPosition()));
	}
	return pairing;
}
List<Pair<String, sf::Vector2i> > DraggableSurface::getRealPositions() const
{
	List<Pair<String, sf::Vector2i> > modules = this->getElementGridPositions();

	for(auto it = modules.begin(); it != modules.end(); ++it)
		it->second = toWorldCoords(it->second);

	return modules;
}
void DraggableSurface::setRealPositions(const List<Pair<String, sf::Vector2i> >& pos)
{
	for(auto it = pos.cbegin(); it != pos.cend(); ++it)
	{
		auto pNewModuleData = dynamic_cast<const ShipModuleData*>(getGame()->getUniverse().getBlueprints().getModuleSPtr(it->first).get());

		DraggableData draggable;
		draggable.metaData = it->first;
		draggable.icon.texName = pNewModuleData->baseDecor.texName;
		draggable.gridPosition = fromWorldCoords(it->second);

		this->addDraggable(draggable);
	}
}
bool DraggableSurface::hasOneAt(const sf::Vector2i& gridPos) const
{
	int count = 0;
	for(auto it = m_widgetList.begin(); it != m_widgetList.end(); ++it)
		if((*it)->getLastGridPosition() == gridPos)
			return true;
	return false;
}
sf::Vector2i DraggableSurface::toWorldCoords(const sf::Vector2i& gridCoord) const
{
	return sf::Vector2i(gridCoord.x - m_gridOffset.x, -(gridCoord.y - m_gridOffset.y));//make negative because view coords vs world
}
sf::Vector2i DraggableSurface::fromWorldCoords(const sf::Vector2i& worldCoord) const
{
	return sf::Vector2i(worldCoord.x + m_gridOffset.x, (-worldCoord.y) + m_gridOffset.y);//make negative because view coords vs world
}
void DraggableSurface::addModuleToEditor(const String& title, sf::Vector2i shipModulePos)
{
	sptr<ShipModuleData> pNewModuleData = sptr<ShipModuleData>(dynamic_cast<ShipModuleData*>(getGame()->getUniverse().getBlueprints().getModuleSPtr(title)->clone()));

	DraggableData draggable;
	draggable.metaData = title;
	draggable.icon.texName = pNewModuleData->baseDecor.texName;
	draggable.gridPosition = fromWorldCoords(shipModulePos);

	this->addDraggable(draggable);
}
bool DraggableSurface::inputHook(const String rCommand, sf::Packet data)
{
	if(rCommand == "buildShipWithConfiguration")
	{
		ShipBuilder::Networked::rebuildFromClient(m_targetShip, this->getRealPositions());
		return true;
	}
	else if(rCommand == "setState")
	{
		m_widgetList.clear();

		List<Pair<String, sf::Vector2i> > modules;
		ShipBuilder::readFromPacket(&m_targetShip, &modules, data);
		setRealPositions(modules);

		return true;
	}
	else if(rCommand == "sell")
	{
		sf::Vector2i target;
		data >> target.x;
		data >> target.y;

		for(auto it = m_widgetList.begin(); it != m_widgetList.end(); ++it)
		{
			Draggable* pDrag = dynamic_cast<Draggable*>((*it).get());
			String draggableBp = pDrag->getMetaData();
			sf::Vector2i draggablePos = pDrag->getGridPosition();
			if(draggablePos == target)
			{
				{//give money
					Resources cost;
					String bpName = pDrag->getMetaData();
					const auto& ref = getGame()->getOverlay().storeData->buttonList;

					for(auto it = ref.cbegin(); it != ref.cend(); ++it)
					{
						if(it->moduleBlueprint == bpName)
						{
							cost = it->cost;
						}
					}
					Resources::ChangeResourcesFromClient(cost.percentOf(0.5f), getGame()->getLocalPlayer().getTeam());
				}
				m_widgetList.erase(it);
				break;
			}
		}

		return true;
	}
	else if(rCommand == "buyModule")
	{
		Player& player = getGame()->getLocalPlayer();

		String title;
		sf::Vector2i shipModulePos;
		Resources cost;

		data >> title;
		data >> shipModulePos.x;
		data >> shipModulePos.y;
		cost.fromPacket(&data);

		const ModuleData* module = getGame()->getUniverse().getBlueprints().getModuleSPtr(title).get();
		if(module != nullptr)
		{
			Resources delta;
			delta.subtract(cost);
			if(player.canChangeResources(delta))
			{
				Resources::ChangeResourcesFromClient(delta, player.getTeam());
				addModuleToEditor(title, shipModulePos);
			}
			else
			{

				// TODO highlight which resource was insufficient
			}
		}
		else
		{
			WARNING;
		}

		return true;
	}
	else if(rCommand == "clearEditor")
	{
		m_widgetList.clear();

		return true;
	}
	else
		return false;
}
