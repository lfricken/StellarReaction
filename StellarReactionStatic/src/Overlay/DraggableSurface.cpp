#include "DraggableSurface.hpp"
#include "Draggable.hpp"
#include "ShipModule.hpp"
#include "BlueprintLoader.hpp"

using namespace leon;
using namespace std;

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
List<std::pair<String, sf::Vector2i> > DraggableSurface::getElementGridPositions() const
{
	List<std::pair<String, sf::Vector2i> > pairing;
	for(auto it = m_widgetList.begin(); it != m_widgetList.end(); ++it)
	{
		Draggable* pDrag = dynamic_cast<Draggable*>((*it).get());
		pairing.push_back(pair<String, sf::Vector2i>(pDrag->getMetaData(), pDrag->getGridPosition()));
	}
	return pairing;
}
bool DraggableSurface::hasOneAt(const sf::Vector2i& gridPos) const
{
	int count = 0;
	for(auto it = m_widgetList.begin(); it != m_widgetList.end(); ++it)
		if((*it)->getLastGridPosition() == gridPos)
			return true;
	return false;
}
Vec2 DraggableSurface::toWorldCoords(const sf::Vector2i& gridCoord) const
{
	return Vec2(static_cast<float>(gridCoord.x - m_gridOffset.x), -static_cast<float>(gridCoord.y - m_gridOffset.y));//make negative because view coords vs world
}
sf::Vector2i DraggableSurface::fromWorldCoords(const Vec2& worldCoord) const
{
	return sf::Vector2i(static_cast<int>(worldCoord.x + m_gridOffset.x), static_cast<int>(-worldCoord.y + m_gridOffset.y));//make negative because view coords vs world
}
bool DraggableSurface::inputHook(const String rCommand, sf::Packet rData)
{
	if(rCommand == "sendState")
	{
		cout << "\nsendState";

		List<std::pair<String, sf::Vector2i> > modules = this->getElementGridPositions();

		sf::Packet pack;
		pack << "rebuild";
		pack << (int32_t)modules.size();

		for(auto it = modules.begin(); it != modules.end(); ++it)
		{
			Vec2 moduleOffset = toWorldCoords(it->second);
			pack << it->first; //name of module
			pack << moduleOffset.x;//grid position x
			pack << moduleOffset.y;//grid position y
		}

		//Tell server that we moved a module. It is then moved there as well.
		Message mes("networkboss", "sendTcpToHost", pack, 0, false);
		game.getCoreIO().recieve(mes);

		return true;
	}
	else if(rCommand == "addItem")
	{
		String title;
		Vec2 shipModulePos;

		rData >> title;
		rData >> shipModulePos.x;
		rData >> shipModulePos.y;

		cout << "\nAddItem: " << shipModulePos.x << shipModulePos.y;

		sptr<ShipModuleData> pNewModuleData = sptr<ShipModuleData>(dynamic_cast<ShipModuleData*>(game.getUniverse().getBlueprints().getModuleSPtr(title)->clone()));

		DraggableData draggable;
		draggable.metaData = title;
		draggable.icon.texName = pNewModuleData->baseDecor.texName;
		draggable.gridPosition = fromWorldCoords(shipModulePos);

		this->addDraggable(draggable);

		return true;
	}
	else if(rCommand == "clear")
	{
		m_widgetList.clear();

		return true;
	}
	else
		return false;
}
