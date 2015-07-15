#ifndef BODYCOMPONENT_HPP
#define BODYCOMPONENT_HPP

#include "stdafx.hpp"
#include "NetworkComponent.hpp"

struct BodyComponentData
{
	BodyComponentData() :
		coords(0,0),
		rotation(0),
		isDynamic(true),
		isBullet(false),
		startAwake(true),
		linearDampening(0.5),
		angularDampening(0.5),
		nwComp()
	{

	}
	b2Vec2 coords;
	float rotation;
	bool isDynamic;
	bool isBullet;
	bool startAwake;
	float linearDampening;
	float angularDampening;
	NetworkComponentData nwComp;
};


class BodyComponent
{
public:
	BodyComponent(const BodyComponentData& rData);
	virtual ~BodyComponent();

	const b2Vec2& getPosition() const;
	b2Body* getBodyPtr();
	NetworkComponent& getNWComp();

	bool isAwake() const;
	void sleep();
	void wake();
	void wake(const b2Vec2& rCoords, float radiansCCW, const b2Vec2& rVel, float angularVel);


protected:
	virtual void pack(sf::Packet& rPacket);
	virtual void unpack(sf::Packet& rPacket);

private:
	NetworkComponent m_nw;
	b2Body* m_pBody;
	b2BodyDef m_bodyDef;

	bool m_awake;
	b2Vec2 m_oldPos;//used for sleep
	float m_oldAngle;//used for sleep
};

#endif // BODYCOMPONENT_HPP
