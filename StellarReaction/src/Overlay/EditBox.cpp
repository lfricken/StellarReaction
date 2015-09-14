#include "EditBox.hpp"

using namespace leon;
using namespace std;

EditBox::EditBox(tgui::Gui& gui, const EditBoxData& rData) : WidgetBase(rData), m_pEditBox(gui)
{
	f_initialize(rData);
}
EditBox::EditBox(tgui::Container& container, const EditBoxData& rData = EditBoxData()) : WidgetBase(rData), m_pEditBox(container)
{
	f_initialize(rData);
}
EditBox::~EditBox()
{

}
void EditBox::f_initialize(const EditBoxData& data)
{
	f_assign(m_pEditBox.get());
	m_pEditBox->load(contentDir() + data.configFile);
	m_pEditBox->setPosition(data.screenCoords);
	m_pEditBox->setText(data.startingText);
	m_pEditBox->setSize(data.size.x, data.size.y);

	m_pEditBox->bindCallbackEx(&EditBox::f_callback, this, tgui::EditBox::AllEditBoxCallbacks);
}




/**PRIVATE**/
void EditBox::f_callback(const tgui::Callback& callback)
{
	if(callback.trigger == tgui::EditBox::MouseEntered)
	{
		f_MouseEntered();
	}
	else if(callback.trigger == tgui::EditBox::LeftMouseClicked)
	{
		f_LeftMouseClicked();
	}
	else if(callback.trigger == tgui::EditBox::MouseLeft)
	{
		f_MouseLeft();
	}
	else if(callback.trigger == tgui::EditBox::TextChanged)
	{
		f_TextChanged();
	}
	else if(callback.trigger == tgui::EditBox::ReturnKeyPressed)
	{
		f_ReturnKeyPressed();
	}
	else
	{
		cout << FILELINE;
	}
}
void EditBox::setText(const std::string& rText)
{
	m_pEditBox->setText(rText);
}
void EditBox::f_MouseEntered()
{
	sf::Packet text;
	std::string stuff = m_pEditBox->getText();
	text << stuff;
	m_io.event(EventType::MouseEntered, 0, text);
}
void EditBox::f_LeftMouseClicked()
{
	sf::Packet text;
	std::string stuff = m_pEditBox->getText();
	text << stuff;
	m_io.event(EventType::LeftMouseClicked, 0, text);
}
void EditBox::f_MouseLeft()
{
	sf::Packet text;
	std::string stuff = m_pEditBox->getText();
	text << stuff;
	m_io.event(EventType::MouseLeft, 0, text);
}
void EditBox::f_TextChanged()
{
	sf::Packet text;
	std::string stuff = m_pEditBox->getText();
	text << stuff;
	m_io.event(EventType::TextChanged, 0, text);
}
void EditBox::f_ReturnKeyPressed()
{
	sf::Packet text;
	std::string stuff = m_pEditBox->getText();
	text << stuff;
	m_io.event(EventType::ReturnKeyPressed, 0, text);
}
void EditBox::f_trigger()
{
	sf::Packet text;
	std::string stuff = m_pEditBox->getText();
	text << stuff;
	m_io.event(EventType::Triggered, 0, text);
}
void EditBox::input(const std::string rCommand, sf::Packet rData)
{
	if(rCommand == "setText")
	{
		std::string text;
		rData >> text;
		setText(text);
	}
	else
		WidgetBase::input(rCommand, rData);
}
