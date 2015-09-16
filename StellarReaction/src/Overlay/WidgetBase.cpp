#include "WidgetBase.hpp"

using namespace leon;
using namespace std;

WidgetBase::WidgetBase(const WidgetBaseData& rData) : m_io(rData.ioComp, &leon::WidgetBase::input, this)
{
	m_startHidden = rData.startHidden;
	m_tempTransparency = rData.transparency;
}
void WidgetBase::f_assign(tgui::Widget* pWidget)
{
	m_pWidget = pWidget;
	if(m_startHidden)
		m_pWidget->hide();

	m_pWidget->setTransparency(m_tempTransparency);
	m_pWidget->bindCallbackEx(&WidgetBase::f_callback, this, tgui::ClickableWidget::AllClickableWidgetCallbacks);
}
WidgetBase::~WidgetBase()
{

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

/**IO**/
void WidgetBase::input(std::string rCommand, sf::Packet rData)
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


/**PRIVATE**/
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
	mouseEnteredHook();
	sf::Packet pack;
	pack << m_io.getName();
	m_io.event(EventType::MouseEntered, 0, pack);
}
void WidgetBase::f_MouseLeft()
{
	mouseLeftHook();
	sf::Packet pack;
	pack << m_io.getName();
	m_io.event(EventType::MouseLeft, 0, pack);
}
void WidgetBase::f_LeftMouseClicked()
{
	mouseClickedHook();
	sf::Packet pack;
	pack << m_io.getName();
	m_io.event(EventType::LeftMouseClicked, 0, pack);
}
void WidgetBase::f_LeftMousePressed()
{
	leftMousePressedHook();
	sf::Packet pack;
	pack << m_io.getName();
	m_io.event(EventType::MousePressed, 0, pack);
}
void WidgetBase::f_LeftMouseReleased()
{
	leftMouseReleasedHook();
	sf::Packet pack;
	pack << m_io.getName();
	m_io.event(EventType::MouseReleased, 0, pack);
}
void WidgetBase::f_trigger()
{
	triggerHook();
	sf::Packet pack;
	pack << m_io.getName();
	m_io.event(EventType::Triggered, 0, pack);
}



/**HOOK FUNCTIONS**/
bool WidgetBase::inputHook(const std::string rCommand, sf::Packet rData)
{
	return false;
}
bool WidgetBase::callbackHook(const tgui::Callback& callback)
{
	return false;
}
void WidgetBase::mouseEnteredHook()
{

}
void WidgetBase::mouseLeftHook()
{

}
void WidgetBase::mouseClickedHook()
{

}
void WidgetBase::leftMousePressedHook()
{

}
void WidgetBase::leftMouseReleasedHook()
{

}
void WidgetBase::triggerHook()
{

}