#pragma once

#include "Game.hpp"

/// Returns the path of the directory that all content is in.
const std::string& contentDir();
/// A packet with nothing in it, which gets used for sending messages without extra data.
const sf::Packet voidPacket;

//TODO these should be defined here, but set somewhere else so changing them doesn't trigger a total recompile.
/// Sets how many pixels (SFML units) is in a meter (Box2D units).
const int scale = 512;//1 box2d unit is scale pixels at 1 zoom
/// Sets how large in pixels a module on a ship should be at 1 zoom.
const int standardModuleSize = 256;
/// The scale on a 1x1 module. If a blueprint says 1x1 module, it actually means this many meters (Box2D unit).
const float sizeScalingFactor = static_cast<float>(standardModuleSize) / static_cast<float>(scale);
/// PI
const float pi = 3.14159f;
/// Useful debugging item.
#define FILELINE "file:{"<<__FILE__<<"}line:{"<<__LINE__<<"}"

/// Globally accessible instance of Game.
/// Declared in main.cpp
extern Game game;


