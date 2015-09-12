#include "NetworkedSelection.hpp"

using namespace leon;
using namespace std;

NetworkedSelection::NetworkedSelection(tgui::Gui& gui, const NetworkedSelectionData& rData) : Panel(gui, rData)
{
	NetworkedSelectionData copy(rData);
	f_initialize(copy, &gui, false);
}
NetworkedSelection::NetworkedSelection(tgui::Container& container, const NetworkedSelectionData& rData = NetworkedSelectionData()) : Panel(container, rData)
{
	NetworkedSelectionData copy(rData);
	f_initialize(copy, &container, true);
}
NetworkedSelection::~NetworkedSelection()
{

}
void NetworkedSelection::f_initialize(NetworkedSelectionData& rData, void* container, bool isContainer)
{
	m_command = rData.command;
	int i = 0;
	sf::Vector2f offset(0,0);
	for(auto it = rData.items.begin(); it != rData.items.end(); ++it, ++i, offset.y += rData.itemSize.y)
	{
		it->buttData.screenCoords += offset;
		it->buttData.size = rData.itemSize;
		sf::Packet pack;
		pack << rData.command;
		it->buttData.ioComp.courierList.back().message.setData(pack);

		auto itLabel = it->labelData.begin();
		for(; itLabel != it->labelData.end(); ++itLabel)
		{
			itLabel->position += offset;
		}


		m_items.push_back(sptr<SelectableItem>(new SelectableItem(*getPanelPtr(), *it)));
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
	else
	{
		//cout << FILELINE;
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
	Panel::input(rCommand, rData);
}
