#pragma once

#include "Game.hpp"

/// Returns the path of the directory that all content is in.
const std::string& contentDir();
/// A packet with nothing in it, which gets used for sending messages without extra data.
const sf::Packet voidPacket;

/// Sets how many pixels (SFML units) is in a meter (Box2D units).
extern const int scale;
/// Sets how large in pixels a module on a ship should be at 1 zoom.
extern const int standardModuleSize;//size in pixels of module at 1 zoom
/// The scale on a 1x1 module. If a blueprint says 1x1 module, it actually means this many meters (Box2D unit).
extern const float sizeScalingFactor;// = 0.5 atm
/// PI
extern const float pi;
/// Useful debugging item.
#define FILELINE "file:{"<<__FILE__<<"}line:{"<<__LINE__<<"}"

/// \brief Globally accessible instance of Game.
///
/// Declared in main.cpp
extern Game game;


