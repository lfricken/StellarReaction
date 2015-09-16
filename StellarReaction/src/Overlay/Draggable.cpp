#include "Draggable.hpp"

using namespace leon;

Draggable::Draggable(tgui::Gui& gui, const DraggableData& rData) : Button(gui, rData)
{
	f_initialize(rData);
}
Draggable::Draggable(tgui::Container& container, const DraggableData& rData) : Button(container, rData)
{
	f_initialize(rData);
}
Draggable::~Draggable()
{

}
void Draggable::f_initialize(const DraggableData& data)
{

}
