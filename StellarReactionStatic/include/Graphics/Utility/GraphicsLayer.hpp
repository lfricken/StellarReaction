#pragma once

#include "stdafx.hpp"


/// Controls the order that an item is drawn to screen, 0 is 
/// drawn lowest, everything after is drawn on top of each subsequent layer.
enum class GraphicsLayer
{
	BackgroundVeryBack = 0,
	BackgroundVeryFar,

	BackgroundFar,//scenery
	BackgroundMid,//scenery
	BackgroundClose,//scenery

	ShipUnder,//fill background
	ShipMiddle,//fill (meter)
	ShipModules,//ship module

	ShipHull,//ship skin

	ShipAppendagesLower,//appendages
	Projectiles,//other
	ShipAppendagesUpper,//tippy top appendages
	Effects,

	Shield,//force fields

	OverlayBottom,//HUD SPRITES LIKE HEALTH AND AMMO
	OverlayMiddle,
	OverlayTop,
	maxLayers,
};

GraphicsLayer ChooseLayer(const std::string& rStr);

