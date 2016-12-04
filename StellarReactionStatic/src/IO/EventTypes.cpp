#include "EventTypes.hpp"



EventType ChooseEvent(const String& rStr)
{
	if(rStr == "Triggered")
		return EventType::Triggered;
	if(rStr == "Health")
		return EventType::Health;
	if(rStr == "ZoomPool")
		return EventType::ZoomPool;
	if(rStr == "AmmoPool")
		return EventType::AmmoPool;
	if(rStr == "MouseLeft")
		return EventType::MouseLeft;
	if(rStr == "MouseEntered")
		return EventType::MouseEntered;
	if(rStr == "LeftMouseClicked")
		return EventType::LeftMouseClicked;
	if(rStr == "TextChanged")
		return EventType::TextChanged;
	if(rStr == "ReturnKeyPressed")
		return EventType::ReturnKeyPressed;
	if(rStr == "OnCount")
		return EventType::OnCount;
	if(rStr == "OnEnter")
		return EventType::OnEnter;
	if(rStr == "OnExit")
		return EventType::OnExit;
	else
	{		
		///ERROR

		return EventType::Triggered;
	}
}
