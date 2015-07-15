#ifndef COURIER_HPP
#define COURIER_HPP

#include "Message.hpp"
#include "Condition.hpp"

/// <summary>
/// This is the standard definition 
/// </summary>
struct Courier
{
	Condition condition;
	Message message;
};

#endif //COURIER_HPP
