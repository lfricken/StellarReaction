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
	sf::Packet data(rData);

	if(rCommand == "toggleHidden")
	{
		toggleHidden(m_pWidget->isVisible());
	}
	else if(rCommand == "setHidden")
	{
		bool mode = false;
		data >> mode;
		toggleHidden(mode);
	}
	else if(rCommand == "toggleEnabled")
	{
		toggleEnabled(!m_pWidget->isEnabled());
	}
	else if(rCommand == "setEnabled")
	{
		bool mode = false;
		data >> mode;
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
	(void)callback;//shutup the compiler about unused
	//override and callbacks here
}
void WidgetBase::f_MouseEntered()
{
	sf::Packet pack;
	pack << m_io.getName();
	m_io.event(EventType::MouseEntered, 0, pack);
}
void WidgetBase::f_MouseLeft()
{
	sf::Packet pack;
	pack << m_io.getName();
	m_io.event(EventType::MouseLeft, 0, pack);
}
void WidgetBase::f_LeftMouseClicked()
{
	sf::Packet pack;
	pack << m_io.getName();
	m_io.event(EventType::LeftMouseClicked, 0, pack);
}
void WidgetBase::f_trigger()
{
	sf::Packet pack;
	pack << m_io.getName();
	m_io.event(EventType::Triggered, 0, pack);
}
