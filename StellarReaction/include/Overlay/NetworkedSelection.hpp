#ifndef NETWORKEDSELECTION_HPP
#define NETWORKEDSELECTION_HPP

#include "WidgetBase.hpp"

namespace leon
{
	struct NetworkedSelectionData : public WidgetBaseData
	{
		NetworkedSelectionData() :
			WidgetBaseData(),
			itemHeight(20),
			command("setShip")
		{
		}
		int itemHeight;
		std::string command;
		std::vector<std::pair<std::string, int> > items;
	};

	class NetworkedSelection : public WidgetBase
	{
	public:
		NetworkedSelection(tgui::Gui& gui, const NetworkedSelectionData& rData);
		NetworkedSelection(tgui::Container& container, const NetworkedSelectionData& rData);
		virtual ~NetworkedSelection();

		void addItem(const std::string& rText, int id);
		void addItems(const std::vector<std::pair<std::string, int> >& rTextList);

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
		tgui::ListBox::Ptr m_pListBox;
		std::string m_command;
	};
}


#endif // NETWORKEDSELECTION_HPP