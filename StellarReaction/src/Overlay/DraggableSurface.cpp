#include "DraggableSurface.hpp"
#include "Draggable.hpp"

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
void DraggableSurface::addDraggable(const DraggableData& rData)
{
	cout << "\n" << sizeof(std::string);
	DraggableData copy = rData;
	copy.gridSize = m_gridSize;
	copy.myPanelOffset = m_pPanel->getAbsolutePosition();
	copy.parentPtr = this;
	sptr<WidgetBase> spDrag(new Draggable(*this->getPanelPtr(), copy));
	this->add(spDrag);
}
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
bool DraggableSurface::connectedGraph() const
{
	//map<sf::Vector2f, bool> spots;
	//for(auto it = m_widgetList.begin(); it != m_widgetList.end(); ++it)
	//	spots[(*it)->getPosition()] = true;
	return false;

}