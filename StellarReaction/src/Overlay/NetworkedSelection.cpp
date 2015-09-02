#include "NetworkedSelection.hpp"

using namespace leon;
using namespace std;

NetworkedSelection::NetworkedSelection(tgui::Gui& gui, const NetworkedSelectionData& rData) : WidgetBase(rData), m_pListBox(gui)
{
	f_initialize(rData);
}
NetworkedSelection::NetworkedSelection(tgui::Container& container, const NetworkedSelectionData& rData = NetworkedSelectionData()) : WidgetBase(rData), m_pListBox(container)
{
	f_initialize(rData);
}
NetworkedSelection::~NetworkedSelection()
{

}
void NetworkedSelection::f_initialize(const NetworkedSelectionData& rData)
{
	m_pListBox->setItemHeight(rData.itemHeight);
	m_command = rData.command;
	f_assign(m_pListBox.get());
	m_pListBox->load(rData.configFile);
	m_pListBox->setPosition(rData.screenCoords);
	m_pListBox->setSize(rData.size.x, rData.size.y);

	addItems(rData.items);

	m_pListBox->bindCallbackEx(&NetworkedSelection::f_callback, this, tgui::EditBox::AllEditBoxCallbacks);
}
void NetworkedSelection::addItem(const std::string& rText, int id)
{
	m_pListBox->addItem(rText, id);
}
void NetworkedSelection::addItems(const std::vector<std::pair<std::string, int> >& rTextList)
{
	for(auto it = rTextList.cbegin(); it != rTextList.cend(); ++it)
		addItem(it->first, it->second);
}



/**PRIVATE**/
void NetworkedSelection::f_GrabInfo(sf::Packet* rPacket)
{
	string stuff = m_pListBox->getSelectedItem();
	int id = m_pListBox->getSelectedItemId();
	(*rPacket) << m_command;
	(*rPacket) << stuff;
	(*rPacket) << id;
}
void NetworkedSelection::f_callback(const tgui::Callback& callback)
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
	else if(callback.trigger == tgui::ListBox::ItemSelected)
	{
		if(m_pListBox->getSelectedItemId() != 0)//make sure nothing isn't selected
			f_ItemSelected();
	}
	else
	{
		cout << FILELINE;
	}
}
void NetworkedSelection::f_ItemSelected()
{
	sf::Packet info;
	f_GrabInfo(&info);
	m_io.event(EventType::Selection, 0, info);
}
void NetworkedSelection::f_MouseEntered()
{
	sf::Packet info;
	f_GrabInfo(&info);
	m_io.event(EventType::MouseEntered, 0, info);
}
void NetworkedSelection::f_LeftMouseClicked()
{
	sf::Packet info;
	f_GrabInfo(&info);
	m_io.event(EventType::LeftMouseClicked, 0, info);
}
void NetworkedSelection::f_MouseLeft()
{
	sf::Packet info;
	f_GrabInfo(&info);
	m_io.event(EventType::MouseLeft, 0, info);
}
void NetworkedSelection::f_trigger()
{
	sf::Packet info;
	f_GrabInfo(&info);
	m_io.event(EventType::Triggered, 0, info);
}
void NetworkedSelection::input(const std::string rCommand, sf::Packet rData)
{
	if(rCommand == "addItem")
	{
		string text;
		int id;
		rData >> text;
		rData >> id;
		addItem(text, id);
	}
	else
		WidgetBase::input(rCommand, rData);
}
