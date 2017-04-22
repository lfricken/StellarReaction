#pragma once

#include "Panel.hpp"
#include "SelectableItem.hpp"

namespace leon
{
	/// Initialize NetworkedSelection
	struct NetworkedSelectionData : public PanelData
	{
		NetworkedSelectionData() :
			PanelData(),
			itemSize(100, 40),
			command("setShip")
		{

		}

		sf::Vector2f itemSize;//size of each item
		String command;//what command to send when an item has been clicked
		List<SelectableItemData> items;
	};


	/// Used to hold a list of SelectableItem that will cause a network event when pressed.
	class NetworkedSelection : public Panel
	{
	public:
		NetworkedSelection(tgui::Gui& gui, const NetworkedSelectionData& rData);
		NetworkedSelection(tgui::Container& container, const NetworkedSelectionData& rData);
		virtual ~NetworkedSelection();

	protected:
		void f_GrabInfo(sf::Packet* rPacket);

		/**events HOOKS**/
		virtual void mouseEnteredHook(sf::Packet& rPack);
		virtual void mouseLeftHook(sf::Packet& rPack);
		virtual void mouseClickedHook(sf::Packet& rPack);
		virtual void leftMousePressedHook(sf::Packet& rPack);
		virtual void leftMouseReleasedHook(sf::Packet& rPack);

		virtual void triggerHook(sf::Packet& rPack);

	private:
		void f_initialize(NetworkedSelectionData& data, void* container, bool isContainer);

		String m_command;//command to send when an item has been selected
		List<sptr<SelectableItem> > m_items;
	};
}
