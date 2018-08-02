#pragma once

#include "IOComponent.hpp"
#include "Globals.hpp"
#include "Universe.hpp"

class GraphicsComponent;
struct DecorationData;


/// \brief A graphics object designed for simulating background
///
/// and far away objects. Can simulate paralax for movement
/// and zoom. Configured in a level.cfg file.
class Decoration final
{
public:
	Decoration(const DecorationData& rData, sptr<GraphicsComponent> pGfx);
	virtual ~Decoration();

	/// \brief Set the position of this Decoration
	///
	/// so that it will apear to the camera at its current position
	/// that it is at that location. (Meaning it accounts for movementScale)
	void setPosition(const Vec2& rWorld);
	/// Set the absolute rotation in radians counterclockwise.
	void setRotation(float radCCW);
	/// Set the current animation and the duration of the animation.
	void setAnimation(const String& rAnimName, float duration);
	/// Control the size of the object. 1 is default. 2 would double the width and height.
	void setScale(float scale);

	/// Returns whether this Decoration wanted a random spawn.
	bool isRandSpawn() const;
	/// Updates the position of this Decoration with respect to the Camera. Used to simulate paralax.
	void updateScaledPosition(const Vec2& rCameraCenter, const Vec2& bottomLeft, const Vec2& topRight, const float zoom, const float dTime);
	/// <summary>
	/// Begins reducing the alpha to 0, given how long it should take.
	/// </summary>
	/// <param name="time"></param>
	void startFadeOut(float time);
	/// <summary>
	/// If there is a fade in time, it starts alpha at 0 and increases it linearly over time
	/// </summary>
	void startFadeIn(float time);

	Vec2 m_lastCameraPos;
	float m_maxZoom;
protected:
	//void input(String rCommand, sf::Packet rData);
private:
	/// Set our velocity to be random.
	void randVel();
	/// Set our spin rate to be random.
	void randSpin();

	/// The graphics object this Decoration controls.
	sptr<GraphicsComponent> m_spGfx;
	//IOComponent m_io;


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
	/// Controlls all paralaxing. Used to calculate paralaxing and zoom.
	float m_zPos;

	bool m_infiniteZ;
	bool m_spawnRandom;
	bool m_repeats;
	bool m_repeatsRandom;

	bool m_isFadingOut;
	float m_totalFadeOutTime;
	float m_fadeOutTimeElapsed;

	bool m_isFadingIn;
	float m_totalFadeInTime;
	float m_fadeInTimeElapsed;

};


/// Data necessary for constructing a Decoration.
struct DecorationData
{
	DecorationData() :
		ioComp(&getGame()->getUniverse().getUniverseIO()),

		minSpinRate(0),
		maxSpinRate(0),
		minVelocity(0, 0),
		maxVelocity(0, 0),

		realPosition(0, 0),
		zPos(0),

		infiniteZ(false),
		repeats(false),
		repeatsRandom(false),
		spawnRandom(false)
	{

	}
	IOComponentData ioComp;

	///Degrees CCW
	float minSpinRate;
	///Degrees CCW
	float maxSpinRate;
	Vec2 minVelocity;
	Vec2 maxVelocity;

	Vec2 realPosition;
	float zPos;

	bool infiniteZ;
	bool repeats;
	bool repeatsRandom;
	bool spawnRandom;

	/// Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};

