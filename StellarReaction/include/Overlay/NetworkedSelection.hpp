#pragma once

#include "Panel.hpp"
#include "SelectableItem.hpp"

namespace leon
{
	struct NetworkedSelectionData : public PanelData
	{
		NetworkedSelectionData() :
			PanelData(),
			itemSize(100, 40),
			command("setShip")
		{

		}

		sf::Vector2f itemSize;//size of each item
		std::string command;//what command to send when an item has been clicked
		std::vector<SelectableItemData> items;
	};

	class NetworkedSelection : public Panel
	{
	public:
		NetworkedSelection(tgui::Gui& gui, const NetworkedSelectionData& rData);
		NetworkedSelection(tgui::Container& container, const NetworkedSelectionData& rData);
		virtual ~NetworkedSelection();

		void addItem();
		void addItems();

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

		std::string m_command;//command to send when an item has been selected
		std::vector<sptr<SelectableItem> > m_items;
	};
}
