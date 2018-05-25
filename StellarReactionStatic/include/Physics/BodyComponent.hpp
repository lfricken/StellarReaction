#ifndef BODYCOMPONENT_HPP
#define BODYCOMPONENT_HPP

#include "stdafx.hpp"
#include "NetworkComponent.hpp"
#include "NonCopyable.hpp"

class RayCastCallback;
class ModuleParent;
enum class Team;

/// Initializes BodyComponent
struct BodyComponentData
{
	BodyComponentData();


	Team team;

	Vec2 coords;
	/// <summary>
	/// Rotation in Degrees CCW
	/// </summary>
	float rotation;
	bool isDynamic;
	bool isBullet;
	bool startAwake;
	bool syncedNetwork;
	float linearDampening;
	float angularDampening;
	NetworkComponentData nwComp;
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};



/// Wrapper for Box2D Body.
class BodyComponent : NonCopyable
{
public:
	BodyComponent(const BodyComponentData& rData);
	virtual ~BodyComponent();
	///Returns a reference to the network component of the body.
	NetworkComponent& getNWComp();
	///Returns true if the body is not sleeping.
	bool isAwake() const;
	///Causes the body to sleep.
	void sleep();
	///Wakes the body from sleep.
	void wake();
	///Wake the body with a given position and velocity.
	void wake(const Vec2& rCoords, float radiansCCW, const Vec2& rVel, float angularVel);
	///Sets the team of the Ship this body represents.
	void setTeam(Team team);
	///Returns the team of the Ship this body represents.
	Team getTeam() const;


	/// <summary>
	/// Returns the position of the body in universe coordinates.
	/// </summary>
	Vec2 getPosition() const;
	/// <summary>
	/// Return angle of body RadCCW.
	/// </summary>
	float getAngle() const;
	/// <summary>
	/// applies force to center of body(Newtons)
	/// </summary>

	void applyTorque(float torqueCCW);

	void applyForce(const Vec2& force);
	void applyForceOffset(const Vec2& force, const Vec2 offset);
	/// <summary>
	/// Return velocity in universal m/s.
	/// </summary>
	Vec2 getLinearVelocity() const;

	void setTransform(const Vec2& pos, float rotCCW);

	void setIgnoreBody(RayCastCallback* ray);

	float getInertia();
	float getMass();

	ModuleParent* moduleParent;
protected:
	virtual void pack(sf::Packet& rPacket);
	virtual void unpack(sf::Packet& rPacket);

private:
	friend struct ModuleData;
	b2Body* getBodyPtr();
	sptr<NetworkComponent> m_nw;
	b2Body* m_pBody;
	b2BodyDef m_bodyDef;

	Vec2 m_oldPos;//used for sleep
	float m_oldAngle;//used for sleep

	Team m_team;
	// 1-4 is blue, green, yellow, pink teams
	// 12 is capture station
	//-784 everything else
};

#endif // BODYCOMPONENT_HPP
