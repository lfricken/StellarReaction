#pragma once

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
		tgui::Button::Ptr m_pButton;//pointer to our button in the gui
		virtual bool callbackHook(const tgui::Callback& callback) final;
		virtual bool callbackHook2(const tgui::Callback& callback);

	private:
		virtual void f_initialize(const ButtonData& data);
	};
}
