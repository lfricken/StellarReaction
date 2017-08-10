#ifndef BODYCOMPONENT_HPP
#define BODYCOMPONENT_HPP

#include "stdafx.hpp"
#include "NetworkComponent.hpp"
#include "NonCopyable.hpp"

enum class Team;

/// Initializes BodyComponent
struct BodyComponentData
{
	BodyComponentData() :
		coords(0,0),
		rotation(0),
		isDynamic(true),
		isBullet(false),
		startAwake(true),
		syncedNetwork(true),
		linearDampening(1.5),
		angularDampening(1.5),
		nwComp()
	{

	}
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
	///Returns the position of the body in world coordinates.
	Vec2 getPosition() const; 
	float getAngle() const;
	///Returns a pointer to the Box2D body object.
	b2Body* getBodyPtr();
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

	Chunk* parent;

protected:
	virtual void pack(sf::Packet& rPacket);
	virtual void unpack(sf::Packet& rPacket);

private:
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
