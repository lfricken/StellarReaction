#pragma once

enum class Team
{
	One = 1,
	Two = 2,
	Three = 3,
	Four = 4,

	Alone = 0,

	///These cannot interact damage wise.
	Neutral = -1,
	Capturable = -2,
	Invalid = -784,
};

