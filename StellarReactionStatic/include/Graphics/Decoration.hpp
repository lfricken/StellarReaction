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

	void updateScaledPosition(const b2Vec2& rCameraCenter);

	void input(std::string rCommand, sf::Packet rData);
protected:
	virtual bool inputHook(std::string rCommand, sf::Packet rData) = 0;//returns true if we handled the command

private:
	GraphicsComponent* m_gfx;
	IOComponent m_io;


	float m_movementScale;
	
	b2Vec2 m_position;//world (where it appears to be due to perspective)
	b2Vec2 m_realPosition;//absolute (where it actually would be in a 3d world)
};



struct DecorationData
{
	DecorationData() :
		ioComp(&game.getUniverse().getUniverseIO()),
		realPosition(0, 0),
		minVelocity(0, 0),
		maxVelocity(0, 0),
		tiled(false),
		movementScale(0)
	{

	}

	IOComponentData ioComp;
	float movementScale;//a stationary target-planar object wouldn't follow the camera, so it would have a
	//movement scale of 0
	//an object far from the target would follow the camera, and have a movement scale <=1
	//an object closer than the target would speed past the camera, and have a movment scale >1

	b2Vec2 realPosition;
	b2Vec2 minVelocity;
	b2Vec2 maxVelocity;
	bool tiled;

	virtual void loadJson(const Json::Value& root);

	virtual Decoration* generate() const = 0;
	virtual DecorationData* clone() const = 0;
};


#endif // DECORATION_HPP
