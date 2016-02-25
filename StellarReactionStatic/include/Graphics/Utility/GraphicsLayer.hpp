#ifndef GRAPHICSLAYER_H
#define GRAPHICSLAYER_H

#include "stdafx.hpp"

/// <summary>
/// Controls the order that an item is drawn to screen, 0 is 
/// drawn lowest, everything after is drawn on top of each subsequent layer
/// </summary>
enum class GraphicsLayer
{
	BackgroundUnmoving1 = 0,
	BackgroundUnmoving2,

	BackgroundFar,//scenery
	Background1,//scenery
	Background2,//scenery
	Background3,//scenery
	Background4,//scenery
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

	Overlay,//HUD SPRITES LIKE HEALTH AND AMMO
	OverlayMiddle,
	maxLayers,
};

GraphicsLayer ChooseLayer(const std::string& rStr);

#endif // GRAPHICSLAYER_H
