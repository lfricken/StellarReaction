#ifndef DECORATION_HPP
#define DECORATION_HPP

#include "IOComponent.hpp"
#include "Globals.hpp"
#include "Universe.hpp"

class GraphicsComponent;
struct DecorationData;

class Decoration
{
public:
	Decoration(const DecorationData& rData, GraphicsComponent* pGfx);
	virtual ~Decoration() = 0;

	void setPosition(const b2Vec2& rWorld);
	void setRotation(float radiansCCW);

	virtual void input(std::string rCommand, sf::Packet rData);
protected:

private:
	IOComponent m_io;

	GraphicsComponent* m_gfx;
	b2Vec2 m_position;//world
	float m_rotation;//radians CCW
};



struct DecorationData
{
	DecorationData() :
		ioComp(game.getUniverse().getUniverseIO())
	{

	}

	IOComponentData ioComp;

	virtual Decoration* generate() const = 0;
	virtual DecorationData* clone() const = 0;
};


#endif // DECORATION_HPP
