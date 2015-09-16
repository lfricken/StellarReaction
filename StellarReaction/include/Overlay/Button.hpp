#ifndef BUTTON_H
#define BUTTON_H

#include "WidgetBase.hpp"

namespace leon
{
	struct ButtonData : public WidgetBaseData
	{
		ButtonData() :
			WidgetBaseData(),
			buttonText("FIXME")
		{
		}

		std::string buttonText;
	};


	class Button : public WidgetBase
	{
	public:
		Button(tgui::Gui& gui, const ButtonData& data);///menubar struct constructor
		Button(tgui::Container& container, const ButtonData& data);
		virtual ~Button();

	protected:

	private:
		virtual void f_initialize(const ButtonData& data);

		tgui::Button::Ptr m_pButton;//pointer to our button in the gui
	};
}
#endif // BUTTON_H
