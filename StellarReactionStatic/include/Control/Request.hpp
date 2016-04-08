#pragma once

/// <summary>
/// Data a controller can request from a chunk (to display via gui, or control zoom etc)
/// </summary>
enum class Request
{
	Zoom,
	MaxZoom,

	Energy,
	MaxEnergy,

	Ballistics,
	MaxBallistics,

	Missiles,
	MaxMissiles,

	Score,
};

