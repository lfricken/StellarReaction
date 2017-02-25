#pragma once

/// Commands a Controller can send to a Chunk
enum class Directive
{
	/**MOVEMENT**/
	Up,
	Down,
	RollCCW,
	RollCW,

	/**== COMBAT ==**/
	FirePrimary,
	FireSecondary,

	/**== MISC. ==**/
	Stealth,
	Teleport,
	Shield,
	ShieldOn,
	ShieldOff,
	Boost,

	Respawn,
	ShowStore,//toggle the store on or off (if there is one near us)

	End,
};


