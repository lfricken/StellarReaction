#include "BodyComponent.hpp"

#include "Globals.hpp"
#include "Universe.hpp"
#include "JSON.hpp"
#include "Convert.hpp"
#include "Debugging.hpp"
#include "RayCastCallback.hpp"



BodyComponentData::BodyComponentData() :
coords(0, 0),
rotation(0),
isDynamic(true),
isBullet(false),
startAwake(true),
syncedNetwork(true),
linearDampening(0.5f),
angularDampening(0.5f),
nwComp()
{

}
void BodyComponentData::loadJson(const Json::Value& root)
{
	GETJSON(coords);
	GETJSON(rotation);
	GETJSON(isDynamic);
	GETJSON(isBullet);
	GETJSON(startAwake);
	GETJSON(linearDampening);
	GETJSON(angularDampening);
}
BodyComponent::BodyComponent(const BodyComponentData& rData)
{
	m_team = rData.team;

	moduleParent = nullptr;

	if(rData.syncedNetwork)
		m_nw.reset(new NetworkComponent(rData.nwComp, &BodyComponent::pack, &BodyComponent::unpack, this, getGame()->getNwBoss().getNWDataFactory()));

	if(rData.isDynamic)
		m_bodyDef.type = b2BodyType::b2_dynamicBody;
	else
		m_bodyDef.type = b2BodyType::b2_staticBody;

	m_bodyDef.position = Convert::universeToWorld(rData.coords);
	m_bodyDef.angle = Convert::degToRad(rData.rotation);
	m_bodyDef.bullet = rData.isBullet;
	m_bodyDef.angularDamping = rData.angularDampening;
	m_bodyDef.linearDamping = rData.linearDampening;

	m_pBody = getGame()->getUniverse().getWorld().CreateBody(&m_bodyDef);
	m_pBody->SetUserData(this);

	if(!rData.startAwake) // if it should be asleep
		sleep(); // then cleanly put it to sleep
}
BodyComponent::~BodyComponent()
{

}
Vec2 BodyComponent::getPosition() const
{
	return Convert::worldToUniverse((Vec2)m_pBody->GetPosition());
}
float BodyComponent::getAngle() const
{
	return m_pBody->GetAngle();
}
b2Body* BodyComponent::getBodyPtr()
{
	return m_pBody;
}
NetworkComponent& BodyComponent::getNWComp()
{
	return *m_nw;
}
Vec2 BodyComponent::getLinearVelocity() const
{
	return Convert::worldToUniverse((Vec2)m_pBody->GetLinearVelocity());
}
void BodyComponent::applyTorque(float torqueCCW)
{
	if(!getGame()->getUniverse().isPaused())
		m_pBody->ApplyTorque(torqueCCW, true);
}
void BodyComponent::applyForce(const Vec2& force)
{
	if(!getGame()->getUniverse().isPaused())
		m_pBody->ApplyForceToCenter(force, true);
}
void BodyComponent::applyForceOffset(const Vec2& force, const Vec2 offset)
{
	if(!getGame()->getUniverse().isPaused())
		m_pBody->ApplyForce(Convert::universeToWorld(force), offset, true);
}
void BodyComponent::setTransform(const Vec2& pos, float rotCCW)
{
	m_pBody->SetTransform(Convert::universeToWorld(pos), rotCCW);
}
void BodyComponent::setIgnoreBody(RayCastCallback* ray)
{
	ray->setIgnoreBody(m_pBody);
}
float BodyComponent::getInertia()
{
	return m_pBody->GetInertia();
}
float BodyComponent::getMass()
{
	return m_pBody->GetMass();
}
void BodyComponent::pack(sf::Packet& rPacket)
{
	//TODO consider switching to universal units
	rPacket << static_cast<float32>(m_pBody->GetPosition().x);
	rPacket << static_cast<float32>(m_pBody->GetPosition().y);
	rPacket << static_cast<float32>(m_pBody->GetLinearVelocity().x);
	rPacket << static_cast<float32>(m_pBody->GetLinearVelocity().y);
	rPacket << static_cast<float32>(m_pBody->GetAngle());
	rPacket << static_cast<float32>(m_pBody->GetAngularVelocity());
}
void BodyComponent::unpack(sf::Packet& rPacket)
{
	//TODO consider switching to universal units
	Vec2 pos;
	Vec2 vel;
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
	b2Body& rm_pBody = *m_pBody;
	m_pBody->SetAwake(true);
	bool awake = m_pBody->IsAwake();
	return awake;
}
void BodyComponent::sleep()
{
	m_oldAngle = getAngle();
	m_oldPos = getPosition();

	m_pBody->SetActive(false);
	m_pBody->SetAwake(false);

	m_pBody->SetAngularVelocity(0);
	m_pBody->SetLinearVelocity(Vec2(0, 0));
	m_pBody->SetTransform(getGame()->getUniverse().getBed(), 0);
}
void BodyComponent::wake()
{
	if(!isAwake())
		wake(m_oldPos, m_oldAngle, Vec2(0,0), 0);
	else
		WARNING;
}
void BodyComponent::wake(const Vec2& rCoords, float radiansCCW, const Vec2& rVel, float angularVel)
{
	m_pBody->SetActive(true);
	m_pBody->SetAwake(true);
	m_pBody->SetTransform(Convert::universeToWorld(rCoords), radiansCCW);
	m_pBody->SetLinearVelocity(Convert::universeToWorld(rVel));
	m_pBody->SetAngularVelocity(angularVel);
}
void BodyComponent::setTeam(Team team)
{
	m_team = team;
}
Team BodyComponent::getTeam() const
{
	return m_team;
}
