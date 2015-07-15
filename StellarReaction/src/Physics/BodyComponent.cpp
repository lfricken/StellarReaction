#include "BodyComponent.hpp"

#include "Globals.hpp"
#include "Universe.hpp"

using namespace std;

BodyComponent::BodyComponent(const BodyComponentData& rData) : m_nw(rData.nwComp, &BodyComponent::pack, &BodyComponent::unpack, this, game.getNwBoss().getNWFactory())
{
	if(rData.isDynamic)
		m_bodyDef.type = b2BodyType::b2_dynamicBody;
	else
		m_bodyDef.type = b2BodyType::b2_staticBody;

	m_bodyDef.position = rData.coords;
	m_bodyDef.bullet = rData.isBullet;
	m_bodyDef.angularDamping = rData.angularDampening;
	m_bodyDef.linearDamping = rData.linearDampening;

	m_pBody = game.getUniverse().getWorld().CreateBody(&m_bodyDef);
	m_pBody->SetUserData(this);

	m_awake = true;//regardless, set us to be awake
	if(!rData.startAwake)//if it should be asleep
		sleep();//then cleanly put it to sleep
}
BodyComponent::~BodyComponent()
{

}
const b2Vec2& BodyComponent::getPosition() const
{
	return m_pBody->GetPosition();
}
b2Body* BodyComponent::getBodyPtr()
{
	return m_pBody;
}
NetworkComponent& BodyComponent::getNWComp()
{
	return m_nw;
}
void BodyComponent::pack(sf::Packet& rPacket)
{
	rPacket << static_cast<float32>(m_pBody->GetPosition().x);
	rPacket << static_cast<float32>(m_pBody->GetPosition().y);
	rPacket << static_cast<float32>(m_pBody->GetLinearVelocity().x);
	rPacket << static_cast<float32>(m_pBody->GetLinearVelocity().y);
	rPacket << static_cast<float32>(m_pBody->GetAngle());
	rPacket << static_cast<float32>(m_pBody->GetAngularVelocity());
}
void BodyComponent::unpack(sf::Packet& rPacket)
{
	b2Vec2 pos;
	b2Vec2 vel;
	float32 posX, posY, velX, velY;
	float32 angle;
	float32 angleVel;

	rPacket >> posX;
	rPacket >> posY;
	rPacket >> velX;
	rPacket >> velY;
	rPacket >> angle;
	rPacket >> angleVel;

	pos.x = posX;
	pos.y = posY;
	vel.x = velX;
	vel.y = velY;

	m_pBody->SetTransform(pos, angle);
	m_pBody->SetLinearVelocity(vel);
	m_pBody->SetAngularVelocity(angleVel);
}
bool BodyComponent::isAwake() const
{
	return m_pBody->IsAwake();
}
void BodyComponent::sleep()
{
	if(isAwake())
	{
		m_oldAngle = m_pBody->GetAngle();
		m_oldPos = m_pBody->GetPosition();

		m_pBody->SetAwake(false);
		m_pBody->SetActive(false);

		m_pBody->SetAngularVelocity(0);
		m_pBody->SetLinearVelocity(b2Vec2(0,0));
		m_pBody->SetTransform(game.getUniverse().getBed(), 0);
	}
	else
		cout << FILELINE;
}
void BodyComponent::wake()
{
	if(!isAwake())
	{
		m_pBody->SetActive(true);
		m_pBody->SetAwake(true);

		m_pBody->SetTransform(m_oldPos, m_oldAngle);
	}
	else
		cout << FILELINE;
}
void BodyComponent::wake(const b2Vec2& rCoords, float radiansCCW, const b2Vec2& rVel, float angularVel)
{
	m_awake = true;

	m_pBody->SetActive(true);
	m_pBody->SetAwake(true);

	m_pBody->SetTransform(rCoords, radiansCCW);
	m_pBody->SetLinearVelocity(rVel);
	m_pBody->SetAngularVelocity(angularVel);
}

