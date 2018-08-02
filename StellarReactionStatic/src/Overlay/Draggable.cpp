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
	m_spDrag = sptr<DragComponent>(new DragComponent(&Draggable::f_update, this, &getGame()->getDragUpdater()));
	m_parentPanelOffset = rData.myPanelOffset;
	m_spDrag->toggleDragging(false);

	m_spPic->disable();//disable to allow us to click through to the button underneath

	if(rData.gridPosition.x != -1 && rData.gridPosition.y != -1)
		trySetGridPosition(rData.gridPosition);

	m_lastGridPosition = this->getGridPosition();
}

const String& Draggable::getMetaData() const
{
	return m_metaData;
}
bool Draggable::trySetGridPosition(const sf::Vector2i& rGridPos)
{
	if(m_pParent->hasOneAt(rGridPos))
	{
		this->setGridPosition(m_lastGridPosition);
		return false;
	}
	else
	{
		this->setGridPosition(rGridPos);
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
	else if(callback.trigger == tgui::ClickableWidget::RightMouseClicked)//clickable widget, not button!
	{
		sf::Vector2i grid = getGridPosition();

		auto mousePos = sf::Mouse::getPosition(getGame()->getWindow());
		auto myPos = getGridPosition();
		sf::Packet data;
		data << mousePos.x;
		data << mousePos.y;
		data << myPos.x;
		data << myPos.y;
		data << true;//center it
		data << true;//mouse off closes (otherwise you need a close button)
		Message selection("return_selection", "show_sellscreen", data, 0, false);
		getGame()->getCoreIO().recieve(selection);
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
/// we were dropped (from being dragged)
/// </summary>
void Draggable::dropped()
{
	m_spDrag->toggleDragging(false);

	sf::Vector2i newGridPos = toGrid(m_pButton->getPosition());
	trySetGridPosition(newGridPos);
}
