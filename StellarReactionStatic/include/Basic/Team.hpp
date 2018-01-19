#pragma once

enum class Team
{
	MinTeam = -1,
	Alone = 0,
	One = 1,
	Two = 2,
	Three = 3,
	Four = 4,
	MaxTeam = 5,


	///These cannot interact damage wise.
	Neutral = -1,
	Capturable = -2,
	Invalid = -784,
};

