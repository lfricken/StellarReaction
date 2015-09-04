#ifndef NETWORKEDSELECTION_HPP
#define NETWORKEDSELECTION_HPP

#include "WidgetBase.hpp"
#include "Panel.hpp"
#include "Button.hpp"

namespace leon
{
	struct LabelData
	{
		sf::Vector2f position;
		sf::Vector2f size;
		sf::Color color;
		std::string text;
	};
	struct SelectableItemData
	{
		std::string texName;
		std::vector<LabelData> labelData;
		std::string id;
	};



	struct SelectableItem
	{
		tgui::Picture::Ptr pic;
		std::vector<sptr<tgui::Label::Ptr> > labels;
		Button butt;
		std::string id;
	};
	struct NetworkedSelectionData : public PanelData
	{
		NetworkedSelectionData() :
			PanelData(),
			itemSize(100, 40),
			command("setShip")
		{
		}
		PanelData internalPanel;
		sf::Vector2f itemSize;
		std::string command;
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
		virtual void f_callback(const tgui::Callback& callback);
		void f_MouseEntered();
		void f_MouseLeft();
		void f_LeftMouseClicked();
		void f_ItemSelected();
		void f_GrabInfo(sf::Packet* rPacket);
		virtual void f_trigger();

		void input(const std::string rCommand, sf::Packet rData);
	private:
		virtual void f_initialize(const NetworkedSelectionData& data);

		std::string m_command;
		std::vector<sptr<SelectableItem> > m_select;
	};
}


#endif // NETWORKEDSELECTION_HPP