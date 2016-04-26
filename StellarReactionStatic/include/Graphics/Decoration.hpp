#pragma once

#include "IOComponent.hpp"
#include "Globals.hpp"
#include "Universe.hpp"

class GraphicsComponent;
struct DecorationData;


/// A graphics object designed for simulating background
/// and far away objects. Can simulate paralax for movement
/// and zoom. Configured in a level.cfg file.
class Decoration final
{
public:
	Decoration(const DecorationData& rData, GraphicsComponent* pGfx);
	virtual ~Decoration();

	/// Set the position of this Decoration 
	/// so that it will apear to the camera at its current position
	/// that it is at that location. (Meaning it accounts for movementScale)
	void setPosition(const Vec2& rWorld);
	/// Set the absolute rotation in radians counterclockwise.
	void setRotation(float radCCW);
	/// Set the current animation and the duration of the animation.
	void setAnimation(const std::string& rAnimName, float duration);
	/// Control the size of the object. 1 is default. 2 would double the width and height.
	void setScale(float scale);

	/// Returns whether this Decoration wanted a random spawn.
	bool isRandSpawn() const;
	/// Updates the position of this Decoration with respect to the Camera. Used to simulate paralax.
	void updateScaledPosition(const Vec2& rCameraCenter, const Vec2& bottomLeft, const Vec2& topRight, const float zoom, const float dTime);

protected:
	void input(std::string rCommand, sf::Packet rData);
private:
	/// Set our velocity to be random.
	void randVel();
	/// Set our spin rate to be random.
	void randSpin();

	/// The graphics object this Decoration controls.
	sptr<GraphicsComponent> m_spGfx;
	IOComponent m_io;

	float m_sizeScale;
	float m_movementScale;

	Vec2 m_lastCameraPos;

	float m_minSpin;
	float m_maxSpin;
	Vec2 m_maxVel;
	Vec2 m_minVel;

	/// Current spin rate.
	float m_spinRate;
	/// Current velocity.
	Vec2 m_velocity;

	/// The position that we WOULD be in a 3D world.
	Vec2 m_realPosition;

	bool m_spawnRandom;
	bool m_repeats;
	bool m_repeatsRandom;
};


/// Data necessary for constructing a Decoration.
struct DecorationData
{
	DecorationData() :
		ioComp(&game.getUniverse().getUniverseIO()),
		sizeScale(0),
		movementScale(0),

		realPosition(0, 0),

		minSpinRate(0),
		maxSpinRate(0),
		minVelocity(0, 0),
		maxVelocity(0, 0),

		repeats(false),
		repeatsRandom(false),
		spawnRandom(false)
	{

	}

	IOComponentData ioComp;

	/// Value used to simulate zooming paralax.
	/// 0 means an object will
	/// appear in the plane of the camera target.
	/// 1 means an object is infinitely far away.
	/// Values in between simulate the middle ground.
	float sizeScale;

	/// Value used to simulate movement paralax.
	/// Examples imagine you are looking out of a speeding car at another speeding car next to you
	/// an object super far      1 (it follows the camera 1:1 and always stays in view) (stars in night sky)
	/// an object kind of far  0.5 (it follows the camera, but moves slowly) (grain silo a mile away)
	/// an object same plane     0 (it doesnt follow the camera) (the ground under the other car)
	/// an object closer      -0.5 (it flies past the camera) (the ground between you and the other car)
	float movementScale;



	Vec2 realPosition;

	float minSpinRate;
	float maxSpinRate;
	Vec2 minVelocity;
	Vec2 maxVelocity;

	bool repeats;
	bool repeatsRandom;
	bool spawnRandom;

	/// Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};

