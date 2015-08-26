#ifndef DECORATION_HPP
#define DECORATION_HPP

#include "IOComponent.hpp"
#include "Globals.hpp"
#include "Universe.hpp"

class GraphicsComponent;
struct DecorationData;

/// <summary>
/// The generic decoration type. Needs to implement a specific type of graphic for GraphicsComponent
/// </summary>
class Decoration
{
public:
	Decoration(const DecorationData& rData, GraphicsComponent* pGfx);
	virtual ~Decoration();

	void setPosition(const b2Vec2& rWorld);
	void setRotation(float radiansCCW);
	void setAnimation(const std::string& rAnimName, float duration);

	void updateScaledPosition(const b2Vec2& rCameraCenter);

	void input(std::string rCommand, sf::Packet rData);
	virtual bool inputHook(std::string rCommand, sf::Packet rData) = 0;//returns true if we handled the command
protected:

private:
	IOComponent m_io;

	float m_movementScale;
	GraphicsComponent* m_gfx;
	b2Vec2 m_position;//world
	b2Vec2 m_initPosition;//absolute
	float m_rotation;//radians CCW
};



struct DecorationData
{
	DecorationData() :
		ioComp(game.getUniverse().getUniverseIO()),
		initPosition(b2Vec2(0,0)),
		movementScale(0.5)
	{

	}

	IOComponentData ioComp;
	float movementScale;
	b2Vec2 initPosition;

	virtual Decoration* generate() const = 0;
	virtual DecorationData* clone() const = 0;
};


#endif // DECORATION_HPP
