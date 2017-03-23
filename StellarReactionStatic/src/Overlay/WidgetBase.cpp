#include "WidgetBase.hpp"
#include "Debugging.hpp"

using namespace leon;
using namespace std;

WidgetBase::WidgetBase(tgui::Gui& gui, const WidgetBaseData& rData) : m_io(rData.ioComp, &leon::WidgetBase::input, this)
{
	pCon = NULL;
	pGui = &gui;
	init(rData);
}
WidgetBase::WidgetBase(tgui::Container& rContainer, const WidgetBaseData& rData) : m_io(rData.ioComp, &leon::WidgetBase::input, this)
{
	pCon = &rContainer;
	pGui = NULL;
	init(rData);
}
void WidgetBase::init(const WidgetBaseData& rData)
{

}
void WidgetBase::f_assign(tgui::Widget* pWidget, const WidgetBaseData& rData)
{
	//Config file
	load(contentDir() + rData.configFile);

	//Start Hidden
	m_pWidget = pWidget;
	if(rData.startHidden)
		m_pWidget->hide();

	//Transparency
	m_pWidget->setTransparency(rData.transparency);

	//Grid Size
	m_gridSize = rData.gridSize;
	//Grid Position
	if(rData.gridPosition != sf::Vector2i(8482, 8482))
		setGridPosition(rData.gridPosition);
	else//Screen Position
		setPosition(rData.screenCoords);
	//Size
	m_pWidget->setSize(rData.size.x, rData.size.y);


	m_pWidget->bindCallbackEx(&WidgetBase::f_callback, this, 4095);//TODO where did this magic number come from?
	//todo probably anding of tgui enums
}
WidgetBase::~WidgetBase()
{
	if(pCon != NULL)
		pCon->remove(m_pWidget);
}
void WidgetBase::enable()
{
	m_pWidget->enable();
}
void WidgetBase::disable()
{
	m_pWidget->disable();
}
void WidgetBase::show()
{
	m_pWidget->show();
	m_pWidget->moveToFront();
}
void WidgetBase::hide()
{
	m_pWidget->hide();
}
void WidgetBase::toggleHidden(bool hidden)
{
	if(hidden)
		hide();
	else
		show();
}
void WidgetBase::toggleEnabled(bool enabled)
{
	if(enabled)
		enable();
	else
		disable();
}
void WidgetBase::setPosition(const sf::Vector2f& realPos)
{
	m_pWidget->setPosition(realPos);
}
const sf::Vector2f& WidgetBase::getPosition() const
{
	return m_pWidget->getPosition();
}
void WidgetBase::setGridPosition(sf::Vector2i gridPos)
{
	gridPos.x *= m_gridSize.x;
	gridPos.y *= m_gridSize.y;

	setPosition((sf::Vector2f)gridPos);
	m_lastGridPosition = getGridPosition();
}
sf::Vector2i WidgetBase::getGridPosition() const
{
	sf::Vector2f gridPos = getPosition();
	gridPos.x /= m_gridSize.x;
	gridPos.y /= m_gridSize.y;
	return (sf::Vector2i)gridPos;
}
sf::Vector2i WidgetBase::getLastGridPosition() const
{
	return m_lastGridPosition;
}
sf::Vector2i WidgetBase::toGrid(sf::Vector2f realPos) const
{
	sf::Vector2f gridHalf(m_gridSize.x / 2.f, m_gridSize.y / 2.f);
	sf::Vector2f calculatedReal = (realPos + gridHalf);
	sf::Vector2i newGridPos;
	newGridPos.x = (int)calculatedReal.x / m_gridSize.x;
	newGridPos.y = (int)calculatedReal.y / m_gridSize.y;

	return newGridPos;
}
sf::Vector2f WidgetBase::fromGrid(sf::Vector2i gridPos) const
{
	return sf::Vector2f((float)(gridPos.x * m_gridSize.x), (float)(gridPos.y * m_gridSize.y));
}
/**IO**/
void WidgetBase::input(String rCommand, sf::Packet rData)
{
	if(inputHook(rCommand, rData))
	{

	}
	else if(rCommand == "toggleHidden")
	{
		toggleHidden(m_pWidget->isVisible());
	}
	else if(rCommand == "setHidden")
	{
		bool mode = false;
		rData >> mode;
		toggleHidden(mode);
	}
	else if(rCommand == "toggleEnabled")
	{
		toggleEnabled(!m_pWidget->isEnabled());
	}
	else if(rCommand == "setEnabled")
	{
		bool mode = false;
		rData >> mode;
		toggleEnabled(mode);
	}
	else if(rCommand == "trigger")
	{
		f_trigger();
	}
	else
	{
		cout << "\nCommand [" << rCommand << "] not found." << FILELINE;
		///ERROR
	}
}
void WidgetBase::f_callback(const tgui::Callback& callback)
{
	if(callbackHook(callback))
	{

	}
	else if(callback.trigger == tgui::Widget::WidgetCallbacks::MouseEntered)
	{
		f_MouseEntered();
	}
	else if(callback.trigger == tgui::Widget::WidgetCallbacks::MouseLeft)
	{
		f_MouseLeft();
	}
	else if(callback.trigger == tgui::ClickableWidget::LeftMouseClicked)
	{
		f_LeftMouseClicked();
	}
	else if(callback.trigger == tgui::ClickableWidget::LeftMousePressed)
	{
		f_LeftMousePressed();
	}
	else if(callback.trigger == tgui::ClickableWidget::LeftMouseReleased)
	{
		f_LeftMouseReleased();
	}
}
void WidgetBase::f_MouseEntered()
{
	sf::Packet pack;
	mouseEnteredHook(pack);
	m_io.event(EventType::MouseEntered, 0, pack);
}
void WidgetBase::f_MouseLeft()
{
	sf::Packet pack;
	mouseLeftHook(pack);
	m_io.event(EventType::MouseLeft, 0, pack);
}
void WidgetBase::f_LeftMouseClicked()
{
	sf::Packet pack;
	mouseClickedHook(pack);
	m_io.event(EventType::LeftMouseClicked, 0, pack);
}
void WidgetBase::f_LeftMousePressed()
{
	sf::Packet pack;
	leftMousePressedHook(pack);
	m_io.event(EventType::MousePressed, 0, pack);
}
void WidgetBase::f_LeftMouseReleased()
{
	sf::Packet pack;
	leftMouseReleasedHook(pack);
	m_io.event(EventType::MouseReleased, 0, pack);
}
void WidgetBase::f_trigger()
{
	sf::Packet pack;
	triggerHook(pack);
	m_io.event(EventType::Triggered, 0, pack);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Hooks
/// </summary>
bool WidgetBase::inputHook(const String rCommand, sf::Packet rData)
{
	return false;
}
bool WidgetBase::callbackHook(const tgui::Callback& callback)
{
	return false;
}
void WidgetBase::mouseEnteredHook(sf::Packet& rPack)
{
	rPack << m_io.getName();
}
void WidgetBase::mouseLeftHook(sf::Packet& rPack)
{
	rPack << m_io.getName();
}
void WidgetBase::mouseClickedHook(sf::Packet& rPack)
{
	rPack << m_io.getName();
}
void WidgetBase::leftMousePressedHook(sf::Packet& rPack)
{
	rPack << m_io.getName();
}
void WidgetBase::leftMouseReleasedHook(sf::Packet& rPack)
{
	rPack << m_io.getName();
}
void WidgetBase::triggerHook(sf::Packet& rPack)
{
	rPack << m_io.getName();
}