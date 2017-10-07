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

		String buttonText;
	};

	/// A clickable Button.
	class Button : public WidgetBase
	{
	public:
		Button(tgui::Gui& gui, const ButtonData& data);///menubar struct constructor
		Button(tgui::Container& container, const ButtonData& data);
		virtual ~Button();

		tgui::Button::Ptr m_pButton;//pointer to our button in the gui
	protected:
		virtual void load(const String& fullFilePath);
		virtual bool inputHook(const String rCommand, sf::Packet rData);
		virtual bool callbackHook(const tgui::Callback& callback) final;
		virtual bool callbackHook2(const tgui::Callback& callback);

	private:
		void f_initialize(const ButtonData& data);
	};
}
