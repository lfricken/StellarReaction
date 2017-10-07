#pragma once

#include "Button.hpp"
#include "NonCopyable.hpp"

namespace leon
{
	/// Label to display on a SelectableItem.
	struct LabelData
	{
		LabelData() :
			textSize(10),
			configFile("TGUI/widgets/Black.conf"),
			text("defaultText"),
			size(40,20),
			color(sf::Color::Red),
			position(10,10)
		{

		}
		int textSize;
		String configFile;
		sf::Vector2f position;
		sf::Vector2f size;
		sf::Color color;
		String text;
	};

	/// Used to intialize SelectableItem
	struct SelectableItemData
	{
		SelectableItemData() :
			texName("menu/default_menu"),
			id("0")
		{
			buttData.screenCoords = sf::Vector2f(0, 0);
			buttData.alpha = 60;
			buttData.configFile = "TGUI/widgets/Transparency.conf";
			buttData.buttonText = "";
		}
		ButtonData buttData;
		String texName;
		List<LabelData> labelData;
		String id;
	};

	/// An object that goes in a NetworkedSelection list. Implements a button to send a special command.
	class SelectableItem : NonCopyable
	{
	public:
		SelectableItem(tgui::Gui& gui, const SelectableItemData& rData);
		SelectableItem(tgui::Container& container, const SelectableItemData& rData);
		virtual ~SelectableItem();

		/// The data held by this SelectableItem.
		String m_id;
		/// The visual part of this.
		tgui::Picture::Ptr m_pic;
		/// A textual TGUI::Label.
		List<sptr<tgui::Label::Ptr> > m_labels;
		/// The Button which handles clicking.
		sptr<Button> m_spButt;

	protected:

	private:
		void f_initialize(const SelectableItemData& rData, void* container, bool isContainer);
	};
}
