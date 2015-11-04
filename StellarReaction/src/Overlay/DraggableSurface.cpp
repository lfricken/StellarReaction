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
	m_gridSize = rData.gridSize;
}
void DraggableSurface::setCountedCoordinates(const std::vector<sf::Vector2f>& rCoords)//which coordinates should we return for getElementPositions
{
	m_validCoords = rCoords;
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
//std::vector<std::pair<std::string, sf::Vector2f> > DraggableSurface::getValidPositions() const
//{
//	//vector<std::pair<std::string, sf::Vector2f> > pairing;
//	//for(auto it = m_widgetList.begin(); it != m_widgetList.end(); ++it)
//	//{
//	//	Draggable* pDrag = dynamic_cast<Draggable*>((*it).get());
//	//	if(std::find(m_validCoords.begin(), m_validCoords.end(), pDrag->getPosition()) != m_validCoords.end())//if it contains
//	//		pairing.push_back(pair<string, sf::Vector2f>(pDrag->getMetaData(), pDrag->getPosition()));
//	//}
//	//return pairing;
//}
std::vector<std::pair<std::string, sf::Vector2f> > DraggableSurface::getElementPositions() const
{
	vector<std::pair<std::string, sf::Vector2f> > pairing;
	for(auto it = m_widgetList.begin(); it != m_widgetList.end(); ++it)
	{
		Draggable* pDrag = dynamic_cast<Draggable*>((*it).get());
		pairing.push_back(pair<string, sf::Vector2f>(pDrag->getMetaData(), pDrag->getPosition()));
	}
	return pairing;
}
bool DraggableSurface::hasOneAt(const sf::Vector2f& gridPos) const
{
	int count = 0;
	for(auto it = m_widgetList.begin(); it != m_widgetList.end(); ++it)
		if((*it)->getPosition() == gridPos)
			++count;
	return (count >= 1);
}
bool DraggableSurface::inputHook(const std::string rCommand, sf::Packet rData)
{
	if(rCommand == "sendState")
	{
		cout << "\nsendState";

		vector<std::pair<string, sf::Vector2f> > modules = this->getElementPositions();

		sf::Packet pack;
		pack << "rebuild";
		pack << (int32_t)modules.size();

		for(auto it = modules.begin(); it != modules.end(); ++it)
		{
			pack << it->first;
			float x = (float)((it->second.x / m_gridSize.x) - 5);
			float y = (float)-((it->second.y / m_gridSize.y) - 5);//negative
			pack << x;
			pack << y;
		}

		Message mes("networkboss", "sendTcpToHost", pack, 0, false);
		game.getCoreIO().recieve(mes);

		return true;
	}
	else if(rCommand == "addItem")
	{
		string title;
		float x;
		float y;

		rData >> title;
		rData >> x;
		rData >> y;

		cout << "\nAddItem: " << x << y;

		sptr<ShipModuleData> pNewModuleData = sptr<ShipModuleData>(dynamic_cast<ShipModuleData*>(game.getUniverse().getBlueprints().getModuleSPtr(title)->clone()));

		DraggableData draggable;
		draggable.metaData = title;
		draggable.icon.texName = pNewModuleData->baseDecor.texName;
		draggable.gridPosition = sf::Vector2f((x + 5), (-y + 5));

		this->addDraggable(draggable);

		return true;
	}
	else
		return false;
}