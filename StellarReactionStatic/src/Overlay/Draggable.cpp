#include "Draggable.hpp"

#include "DragComponent.hpp"
#include "DraggableSurface.hpp"
using namespace leon;

Draggable::Draggable(tgui::Gui& container, const DraggableData& rData) : m_spPic(new Picture(container, rData.icon)), Button(container, rData)
{
	f_initialize(rData);
}
Draggable::Draggable(tgui::Container& container, const DraggableData& rData) : m_spPic(new Picture(container, rData.icon)), Button(container, rData)
{
	f_initialize(rData);
}
Draggable::~Draggable()
{

}
void Draggable::f_initialize(const DraggableData& rData)
{
	m_metaData = rData.metaData;
	m_pParent = rData.parentPtr;
	m_spDrag = sptr<DragComponent>(new DragComponent(&Draggable::f_update, this, &game.getDragUpdater()));
	m_parentPanelOffset = rData.myPanelOffset;
	m_spDrag->toggleDragging(false);
	m_gridSize = rData.gridSize;

	m_spPic->disable();//disable to allow us to click through to the button underneath

	if(rData.gridPosition.x != -1 && rData.gridPosition.y != -1)
		trySetPosition(sf::Vector2f(rData.gridPosition.x*rData.gridSize.x, rData.gridPosition.y*rData.gridSize.y));

	m_lastPosition = this->getPosition();
}
sf::Vector2f Draggable::getGridPosition() const
{
	sf::Vector2f cur = m_pButton->getPosition();
	cur.x /= m_gridSize.x;
	cur.y /= m_gridSize.y;
	return cur;
}
const std::string& Draggable::getMetaData() const
{
	return m_metaData;
}
void Draggable::setGridPosition(const sf::Vector2f& rPos)
{
	sf::Vector2f copy(rPos);
	copy.x *= m_gridSize.x;
	copy.y *= m_gridSize.y;
	this->trySetPosition(copy);
}
bool Draggable::trySetPosition(const sf::Vector2f& rPos)
{
	if(m_pParent->hasOneAt(rPos))
	{
		this->setPosition(m_lastPosition);
		return false;
	}
	else
	{
		this->setPosition(rPos);
		return true;
	}
}
void Draggable::setPosition(const sf::Vector2f& rPos)
{
	m_spPic->setPosition(rPos);
	m_pButton->setPosition(rPos);
}
bool Draggable::callbackHook2(const tgui::Callback& callback)
{
	if(callback.trigger == tgui::Button::LeftMousePressed)
	{
		m_spDrag->toggleDragging(true);
		return true;
	}
	else if(callback.trigger == tgui::Button::LeftMouseReleased)
	{
		if(m_spDrag->isDragging())
			dropped();
		return true;
	}
	else
		return false;
}
void Draggable::f_update(const sf::Vector2f& rPos)
{
	sf::Vector2f offsetToCenter = m_pButton->getSize();
	offsetToCenter = sf::Vector2f(offsetToCenter.x / 2, offsetToCenter.y / 2);
	this->setPosition(rPos - m_parentPanelOffset - offsetToCenter);
}
/// <summary>
/// round our position to nearest snap
/// </summary>
sf::Vector2f Draggable::getClosestPosition(const sf::Vector2f& rCurrent)
{
	sf::Vector2f gridHalf(m_gridSize.x / 2, m_gridSize.y / 2);
	sf::Vector2f calculated = (rCurrent + gridHalf);
	calculated.x = static_cast<float>(static_cast<int>(calculated.x) / static_cast<int>(m_gridSize.x));
	calculated.y = static_cast<float>(static_cast<int>(calculated.y) / static_cast<int>(m_gridSize.y));

	calculated.x *= m_gridSize.x;
	calculated.y *= m_gridSize.y;

	return calculated;
}
/// <summary>
/// we were dropped (from being dragged)
/// </summary>
void Draggable::dropped()
{
	m_spDrag->toggleDragging(false);

	sf::Vector2f newPos = getClosestPosition(m_pButton->getPosition());
	if(this->trySetPosition(newPos))
		m_lastPosition = this->getPosition();
}
