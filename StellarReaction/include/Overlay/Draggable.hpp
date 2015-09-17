#pragma once

#include "Button.hpp"

namespace leon
{
	struct DraggableData : public ButtonData
	{
		DraggableData() :
			ButtonData()
		{
			buttonText = "";
		}

	};


	class Draggable : public Button
	{
	public:
		Draggable(tgui::Gui& gui, const DraggableData& rData);
		Draggable(tgui::Container& container, const DraggableData& rData);
		virtual ~Draggable();

		sf::Vector2f getPosition() const;
		void setPosition(const sf::Vector2f& rPos);

	protected:
		virtual bool callbackHook2(const tgui::Callback& callback);

	private:
		void f_initialize(const DraggableData& data);
	};
}
