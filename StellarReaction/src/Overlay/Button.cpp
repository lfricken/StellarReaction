#include "Button.hpp"

using namespace leon;


Button::Button(tgui::Gui& gui, const ButtonData& rData) : WidgetBase(rData), m_pButton(gui)
{
	f_initialize(rData);
}
Button::Button(tgui::Container& container, const ButtonData& rData = ButtonData()) : WidgetBase(rData), m_pButton(container)
{
	f_initialize(rData);
}
Button::~Button()
{

}
void Button::f_initialize(const ButtonData& rData)
{
	f_assign(m_pButton.get());
	m_pButton->load(rData.configFile);
	m_pButton->setPosition(rData.screenCoords);
	m_pButton->setText(rData.buttonText);
	m_pButton->setSize(rData.size.x, rData.size.y);

	m_pButton->bindCallbackEx(&Button::f_callback, this, tgui::Button::AllButtonCallbacks);
}
void Button::input(const std::string rCommand, sf::Packet rData)
{
	WidgetBase::input(rCommand, rData);
}
void Button::f_callback(const tgui::Callback& callback)
{
	if(callback.trigger == tgui::Button::MouseEntered)
	{
		f_MouseEntered();
	}
	else if(callback.trigger == tgui::Button::LeftMouseClicked)
	{
		f_LeftMouseClicked();
	}
	else if(callback.trigger == tgui::Button::MouseLeft)
	{
		f_MouseLeft();
	}
}
void Button::f_trigger()
{
	sf::Packet pack;
	pack << m_io.getName();
	m_io.event(EventType::Triggered, 0, pack);
	f_LeftMouseClicked();
}
