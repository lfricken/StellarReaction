#pragma once

/// \brief Things a controller can request from a chunk.
///
/// Usually for displaying as data on the HUD.
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

