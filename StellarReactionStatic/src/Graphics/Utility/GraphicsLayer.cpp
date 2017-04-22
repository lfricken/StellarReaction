#include "GraphicsLayer.hpp"

GraphicsLayer ChooseLayer(const String& rStr)
{
	if(rStr == "Back8")
		return GraphicsLayer::Back8;
	if(rStr == "Back7")
		return GraphicsLayer::Back7;
	if(rStr == "Back6")
		return GraphicsLayer::Back6;
	if(rStr == "Back5")
		return GraphicsLayer::Back5;
	if(rStr == "Back4")
		return GraphicsLayer::Back4;
	if(rStr == "Back3")
		return GraphicsLayer::Back3;
	if(rStr == "Back2")
		return GraphicsLayer::Back2;
	if(rStr == "Back1")
		return GraphicsLayer::Back1;


	if(rStr == "BackgroundFar")
		return GraphicsLayer::BackgroundFar;
	if(rStr == "BackgroundMid")
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
