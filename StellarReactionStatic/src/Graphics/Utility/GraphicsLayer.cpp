#include "GraphicsLayer.hpp"

GraphicsLayer ChooseLayer(const std::string& rStr)
{
	if(rStr == "BackgroundVeryBack")
		return GraphicsLayer::BackgroundVeryBack;
	if(rStr == "BackgroundVeryFar")
		return GraphicsLayer::BackgroundVeryFar;

	if(rStr == "BackgroundFar")
		return GraphicsLayer::BackgroundFar;
	if(rStr == "BackgroundFar")
		return GraphicsLayer::BackgroundMid;
	if(rStr == "BackgroundClose")
		return GraphicsLayer::BackgroundClose;


	if(rStr == "ShipUnder")
		return GraphicsLayer::ShipUnder;
	if(rStr == "ShipMiddle")
		return GraphicsLayer::ShipMiddle;
	if(rStr == "ShipModules")
		return GraphicsLayer::ShipModules;


	if(rStr == "ShipHull")
		return GraphicsLayer::ShipHull;


	if(rStr == "ShipAppendagesLower")
		return GraphicsLayer::ShipAppendagesLower;
	if(rStr == "Projectiles")
		return GraphicsLayer::Projectiles;
	if(rStr == "ShipAppendagesUpper")
		return GraphicsLayer::ShipAppendagesUpper;
	if(rStr == "Effects")
		return GraphicsLayer::Effects;

	if(rStr == "Shield")
		return GraphicsLayer::Shield;


	if(rStr == "OverlayBottom")
		return GraphicsLayer::OverlayBottom;
	if(rStr == "OverlayMiddle")
		return GraphicsLayer::OverlayMiddle;
	if(rStr == "OverlayTop")
		return GraphicsLayer::OverlayTop;
	if(rStr == "maxLayers")
		return GraphicsLayer::maxLayers;
	else
	{
		///ERROR
		return GraphicsLayer::OverlayTop;
	}
}
