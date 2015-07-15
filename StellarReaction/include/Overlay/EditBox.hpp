#ifndef EDITBOX_H
#define EDITBOX_H

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
		virtual void f_callback(const tgui::Callback& callback);
		void f_MouseEntered();
		void f_MouseLeft();
		void f_LeftMouseClicked();
		void f_TextChanged();
		void f_ReturnKeyPressed();
		virtual void f_trigger();

		void input(const std::string rCommand, sf::Packet rData);
	private:
		virtual void f_initialize(const EditBoxData& data);

		tgui::EditBox::Ptr m_pEditBox;//pointer to our editbox inside gui
	};
}
#endif // EDITBOX_H
