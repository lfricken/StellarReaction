#ifndef COURIER_HPP
#define COURIER_HPP

#include "Message.hpp"
#include "Condition.hpp"

/*
* A condition for when to send a message
* and the message to send
*/
struct Courier
{
	Condition condition;
	Message message;
};

#endif //COURIER_HPP
