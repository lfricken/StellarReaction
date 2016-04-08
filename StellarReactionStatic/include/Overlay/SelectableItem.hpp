#pragma once

#include "Button.hpp"
#include "NonCopyable.hpp"

namespace leon
{
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
		std::string configFile;
		sf::Vector2f position;
		sf::Vector2f size;
		sf::Color color;
		std::string text;
	};


	struct SelectableItemData
	{
		SelectableItemData() :
			texName("menu/default_menu.png"),
			id("0")
		{
			buttData.screenCoords = sf::Vector2f(0, 0);
			buttData.transparency = 60;
			buttData.configFile = "TGUI/widgets/Transparency.conf";
			buttData.buttonText = "";
		}
		ButtonData buttData;
		std::string texName;
		std::vector<LabelData> labelData;
		std::string id;
	};


	class SelectableItem : NonCopyable
	{
	public:
		SelectableItem(tgui::Gui& gui, const SelectableItemData& rData);
		SelectableItem(tgui::Container& container, const SelectableItemData& rData);
		virtual ~SelectableItem();

		std::string m_id;
		tgui::Picture::Ptr m_pic;
		std::vector<sptr<tgui::Label::Ptr> > m_labels;
		sptr<Button> m_spButt;

	protected:

	private:
		void f_initialize(const SelectableItemData& rData, void* container, bool isContainer);
	};
}
