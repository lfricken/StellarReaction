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
	void setScale(float scale);

	bool hasAbsoluteSize() const;

	void updateScaledPosition(const b2Vec2& rCameraCenter);

	void input(std::string rCommand, sf::Packet rData);
	virtual bool inputHook(std::string rCommand, sf::Packet rData) = 0;//returns true if we handled the command

	//Evan
	b2Vec2 velocity;
	bool m_repeats;
	Timer velocityTimer;
	b2Vec2 dimensions;
	b2Vec2 num_in_layer;

protected:

private:
	IOComponent m_io;

	bool m_isAbsoluteSize;
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
		initPosition(0,0),
		velocity(0,0),
		repeats(false),
		movementScale(0),
		isAbsoluteSize(false)
	{

	}

	IOComponentData ioComp;
	float movementScale;//a stationary target-planar object wouldn't follow the camera, so it would have a
	//movement scale of 0
	//an object far from the target would follow the camera, and have a movement scale <=1
	//an object closer than the target would speed past the camera, and have a movment scale >1
	bool isAbsoluteSize;
	b2Vec2 initPosition;
	b2Vec2 velocity;
	bool repeats;

	virtual Decoration* generate() const = 0;
	virtual DecorationData* clone() const = 0;

	//Evan
	b2Vec2 num_in_layer;
	b2Vec2 dimensions;
};


#endif // DECORATION_HPP
