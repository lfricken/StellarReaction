#include "NetworkedSelection.hpp"

using namespace leon;


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
	/// <summary>
	/// for each item, set the command on that button to execute the command, and send the rData.command
	/// </summary>
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
void NetworkedSelection::f_GrabInfo(sf::Packet* rPacket)
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Hooks
/// </summary>
void NetworkedSelection::mouseEnteredHook(sf::Packet& rPack)
{
	f_GrabInfo(&rPack);
}
void NetworkedSelection::mouseLeftHook(sf::Packet& rPack)
{
	f_GrabInfo(&rPack);
}
void NetworkedSelection::mouseClickedHook(sf::Packet& rPack)
{
	f_GrabInfo(&rPack);
}
void NetworkedSelection::leftMousePressedHook(sf::Packet& rPack)
{
	f_GrabInfo(&rPack);
}
void NetworkedSelection::leftMouseReleasedHook(sf::Packet& rPack)
{
	f_GrabInfo(&rPack);
}
void NetworkedSelection::triggerHook(sf::Packet& rPack)
{
	f_GrabInfo(&rPack);
}
