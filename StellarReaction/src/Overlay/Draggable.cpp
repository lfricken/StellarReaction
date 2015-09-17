#include "Draggable.hpp"

using namespace leon;

Draggable::Draggable(tgui::Gui& container, const DraggableData& rData) : Button(container, rData)
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
void Draggable::f_initialize(const DraggableData& rData)
{

}
sf::Vector2f Draggable::getPosition() const
{
	return m_pButton->getPosition();
}
void Draggable::setPosition(const sf::Vector2f& rPos)
{
	m_pButton->setPosition(rPos);
}
bool Draggable::callbackHook2(const tgui::Callback& callback)
{
	std::cout << "\nSomething happened.";
	return false;
}
