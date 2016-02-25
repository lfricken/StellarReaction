#pragma once

#include "WidgetBase.hpp"

namespace leon
{
	struct EditBoxData : public WidgetBaseData
	{
		EditBoxData() :
			WidgetBaseData(),
			startingText("FIXME")
		{
		}

		std::string startingText;
	};


	class EditBox : public WidgetBase
	{
	public:
		EditBox(tgui::Gui& gui, const EditBoxData& data);///menubar struct constructor
		EditBox(tgui::Container& container, const EditBoxData& data);
		virtual ~EditBox();

		void setText(const std::string& rText);//removes all text in editbox

	protected:
		void f_TextChanged();
		void f_ReturnKeyPressed();

		/**events HOOKS**/
		virtual bool inputHook(const std::string rCommand, sf::Packet rData);
		virtual bool callbackHook(const tgui::Callback& callback);

		virtual void mouseEnteredHook(sf::Packet& rPack);
		virtual void mouseLeftHook(sf::Packet& rPack);
		virtual void mouseClickedHook(sf::Packet& rPack);
		virtual void leftMousePressedHook(sf::Packet& rPack);
		virtual void leftMouseReleasedHook(sf::Packet& rPack);

		virtual void triggerHook(sf::Packet& rPack);

	private:
		void f_initialize(const EditBoxData& data);

		tgui::EditBox::Ptr m_pEditBox;//pointer to our editbox inside gui
	};
}
