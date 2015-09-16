#pragma once

#include "Button.hpp"

namespace leon
{
	struct DraggableData : public ButtonData
	{
		DraggableData() :
			ButtonData()
		{

		}

	};


	class Draggable : public Button
	{
	public:
		Draggable(tgui::Gui& gui, const DraggableData& data);///menubar struct constructor
		Draggable(tgui::Container& container, const DraggableData& data);
		virtual ~Draggable();
	protected:

		virtual void f_LeftMousePressed();
		virtual void f_LeftMouseReleased();
		virtual void f_MouseLeft();
	private:
		void f_initialize(const DraggableData& data);
	};
}
