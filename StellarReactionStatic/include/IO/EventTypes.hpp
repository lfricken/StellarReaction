#pragma once

#include "stdafx.hpp"

/// A list of event types, as well as the type of value they are intended to check for.
enum class EventType
{
	/**UNIVERSE**/
	Triggered,//called when a relay is told to trigger, pass in GuestCount
	Health,//called when health changes, pass in current
	ZoomPool,//called when current changes, pass in current
	AmmoPool,//called when current changes, pass in current
	/**UNIVERSE**/

	/**GUI**/
	MouseLeft,//called when mouse left our AABB, pass in
	MouseEntered,//called when mouse entered our AABB, pass in

	LeftMouseClicked,//called when we are leftClicked on, pass in
	LeftMousePressed,
	LeftMouseReleased,
	RightMouseClicked,//called when we are rightClicked on, pass in
	RightMousePressed,
	RightMouseReleased,

	TextChanged,//called when text changes in editbox, pass in TextInBox
	ReturnKeyPressed,//called when enter pressed in editbox, pass in TextInBox
	Selection,//called when someone selects from a list
	/**GUI**/

	/**TRIGGERS**/
	OnCount,//called when GuestCount changes, pass in GuestCount
	OnEnter,//called when an object enters us, pass in GuestCount
	OnExit,//called when an object leaves a trigger, pass in GuestCount
	/**TRIGGERS**/

	None,
};
/// \brief Converts the EventType String to enum class.
///
/// Used for loading JSON.
EventType ChooseEvent(const String& rStr);

