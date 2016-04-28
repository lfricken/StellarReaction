#pragma once

#include "WidgetBase.hpp"

namespace leon
{
	/// Data to initialize a Button.
	struct ButtonData : public WidgetBaseData
	{
		ButtonData() :
			WidgetBaseData(),
			buttonText("FIXME")
		{
		}

		std::string buttonText;
	};

	/// A clickable Button.
	class Button : public WidgetBase
	{
	public:
		Button(tgui::Gui& gui, const ButtonData& data);///menubar struct constructor
		Button(tgui::Container& container, const ButtonData& data);
		virtual ~Button();

	protected:
		virtual bool inputHook(const std::string rCommand, sf::Packet rData);
		tgui::Button::Ptr m_pButton;//pointer to our button in the gui
		virtual bool callbackHook(const tgui::Callback& callback) final;
		virtual bool callbackHook2(const tgui::Callback& callback);

	private:
		void f_initialize(const ButtonData& data);
	};
}
