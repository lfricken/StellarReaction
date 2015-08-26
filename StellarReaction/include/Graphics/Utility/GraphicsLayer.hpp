#ifndef GRAPHICSLAYER_H
#define GRAPHICSLAYER_H

#include "stdafx.hpp"

enum class GraphicsLayer
{
	BackgroundUnmoving = 0,
	BackgroundSlow,

	BackgroundFar,//scenery
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
