#pragma once

/// <summary>
/// Commands a controller can send to a Chunk
/// </summary>
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
	Boost,

	Respawn,
	ShowStore,//toggle the store on or off (if there is one near us)

	End,
};


