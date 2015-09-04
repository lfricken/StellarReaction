#include "NetworkedSelection.hpp"

using namespace leon;
using namespace std;

NetworkedSelection::NetworkedSelection(tgui::Gui& gui, const NetworkedSelectionData& rData) : Panel(gui, rData)
{
	f_initialize(rData);
}
NetworkedSelection::NetworkedSelection(tgui::Container& container, const NetworkedSelectionData& rData = NetworkedSelectionData()) : Panel(container, rData)
{
	f_initialize(rData);
}
NetworkedSelection::~NetworkedSelection()
{

}
void NetworkedSelection::f_initialize(const NetworkedSelectionData& rData)
{
	m_command = rData.command;
	int i = 0;
	for(auto it = rData.items.begin(); it != rData.items.end(); ++it)
	{
		ButtonData select;
		select.buttonText = "";
		select.screenCoords = sf::Vector2f(0, i*rData.itemSize.y);
		select.size = rData.itemSize;
		select.ioComp.name = std::to_string(i);
		Courier buttonMes;
		buttonMes.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
		buttonMes.message.reset(m_io.getPosition(), "itemClicked", voidPacket, 0, true);
		select.ioComp.courierList.push_back();

		++i;
	}
}
void NetworkedSelection::addItem()
{

}
void NetworkedSelection::addItems()
{

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
