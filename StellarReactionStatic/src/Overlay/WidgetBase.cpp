#include "WidgetBase.hpp"

using namespace leon;
using namespace std;

WidgetBase::WidgetBase(tgui::Gui& gui, const WidgetBaseData& rData) : m_io(rData.ioComp, &leon::WidgetBase::input, this)
{
	m_startHidden = rData.startHidden;
	m_tempTransparency = rData.transparency;
	pCon = NULL;
	pGui = &gui;
}
WidgetBase::WidgetBase(tgui::Container& rContainer, const WidgetBaseData& rData) : m_io(rData.ioComp, &leon::WidgetBase::input, this)
{
	m_startHidden = rData.startHidden;
	m_tempTransparency = rData.transparency;
	pCon = &rContainer;
	pGui = NULL;
}
void WidgetBase::f_assign(tgui::Widget* pWidget)
{
	m_pWidget = pWidget;
	if(m_startHidden)
		m_pWidget->hide();

	m_pWidget->setTransparency(m_tempTransparency);
	m_pWidget->bindCallbackEx(&WidgetBase::f_callback, this, 4095);
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
void WidgetBase::setPosition(const sf::Vector2f& newPos)
{
	m_pWidget->setPosition(newPos);
}
const sf::Vector2f& WidgetBase::getPosition() const
{
	return m_pWidget->getPosition();
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