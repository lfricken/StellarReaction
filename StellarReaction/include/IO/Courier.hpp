#ifndef COURIER_HPP
#define COURIER_HPP

#include "Message.hpp"
#include "Condition.hpp"

/// <summary>
/// A condition for when to send a message
/// The message to send
/// </summary>
struct Courier
{
	Condition condition;
	Message message;
};

#endif //COURIER_HPP
