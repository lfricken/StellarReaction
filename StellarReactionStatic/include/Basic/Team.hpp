#pragma once

enum class Team
{
	One = 1,
	Two = 2,
	Three = 3,
	Four = 4,

	Neutral = 0,//cannot send or recieve damage
	Invinicible = -1,///Damaged by no one.
	Alone = -2,

	Capturable = -3,

	Invalid = -784,
};



