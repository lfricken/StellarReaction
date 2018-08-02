#pragma once

#include "Game.hpp"

/// Returns the path of the directory that all content is in.
const String& contentDir();
/// A packet with nothing in it, which gets used for sending messages without extra data.
const sf::Packet voidPacket;

/// <summary>
/// Globally accessible instance of getGame()->
/// Declared in main.cpp
/// </summary>
extern Game* getGame(Game* instance = nullptr);

