#ifndef GLOBALS_H
#define GLOBALS_H


#include "Game.hpp"


const std::string& contentDir();//directory of content
const sf::Packet voidPacket;// a void packet when we don't want to send anything

extern const int scale;//1 box2d unit is scale pixels at 1 zoom
extern const int standardModuleSize;//size in pixels of module at 1 zoom
extern const float sizeScalingFactor;// = 0.5 atm
extern const float pi;


#define FILELINE "file:{"<<__FILE__<<"}line:{"<<__LINE__<<"}"

extern Game game;

#endif // GLOBALS_H
