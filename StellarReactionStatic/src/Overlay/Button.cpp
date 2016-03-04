#include "Button.hpp"
#include "Player.hpp"

using namespace leon;

Button::Button(tgui::Gui& gui, const ButtonData& rData) : WidgetBase(gui, rData), m_pButton(gui)
{
	f_initialize(rData);
}
Button::Button(tgui::Container& container, const ButtonData& rData = ButtonData()) : WidgetBase(container, rData), m_pButton(container)
{
	f_initialize(rData);
}
Button::~Button()
{

}
bool Button::inputHook(const std::string rCommand, sf::Packet rData)
{
	if (rCommand == "setText")
	{
		std::string text;
		rData >> text;
		m_pButton->setText(text);
		return true;
	}
	else
		return false;
}
void Button::f_initialize(const ButtonData& rData)
{
	f_assign(m_pButton.get());
	m_pButton->load(contentDir() + rData.configFile);
	m_pButton->setPosition(rData.screenCoords);
	m_pButton->setText(rData.buttonText);
	m_pButton->setSize(rData.size.x, rData.size.y);
}
bool Button::callbackHook(const tgui::Callback& callback)
{
	if(callbackHook2(callback))
	{
		return true;
	}
	else
		return false;
}
bool Button::callbackHook2(const tgui::Callback& callback)
{
	return false;
}
