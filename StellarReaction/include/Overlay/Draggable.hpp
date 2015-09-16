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

	private:
		void f_initialize(const DraggableData& data);
	};
}
