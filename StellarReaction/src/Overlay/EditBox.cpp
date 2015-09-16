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
}
bool EditBox::inputHook(const std::string rCommand, sf::Packet rData)
{
	if(rCommand == "setText")
	{
		std::string text;
		rData >> text;
		setText(text);
		return true;
	}
	return false;
}
void EditBox::setText(const std::string& rText)
{
	m_pEditBox->setText(rText);
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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Hooks
/// </summary>
bool EditBox::callbackHook(const tgui::Callback& callback)
{
	if(callback.trigger == tgui::EditBox::TextChanged)
	{
		f_TextChanged();
		return true;
	}
	else if(callback.trigger == tgui::EditBox::ReturnKeyPressed)
	{
		f_ReturnKeyPressed();
		return true;
	}
	else
		return false;
}
void EditBox::mouseEnteredHook(sf::Packet& rPack)
{
	rPack << m_pEditBox->getText();
}
void EditBox::mouseLeftHook(sf::Packet& rPack)
{
	rPack << m_pEditBox->getText();
}
void EditBox::mouseClickedHook(sf::Packet& rPack)
{
	rPack << m_pEditBox->getText();
}
void EditBox::leftMousePressedHook(sf::Packet& rPack)
{
	rPack << m_pEditBox->getText();
}
void EditBox::leftMouseReleasedHook(sf::Packet& rPack)
{
	rPack << m_pEditBox->getText();
}
void EditBox::triggerHook(sf::Packet& rPack)
{
	rPack << m_pEditBox->getText();
}
