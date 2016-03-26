#include "BodyComponent.hpp"

#include "Globals.hpp"
#include "Universe.hpp"
#include "JSON.hpp"
#include "Convert.hpp"

using namespace std;

BodyComponent::BodyComponent(const BodyComponentData& rData) : m_nw(rData.nwComp, &BodyComponent::pack, &BodyComponent::unpack, this, game.getNwBoss().getNWFactory())
{
	if(rData.isDynamic)
		m_bodyDef.type = b2BodyType::b2_dynamicBody;
	else
		m_bodyDef.type = b2BodyType::b2_staticBody;

	m_bodyDef.position = rData.coords;
	m_bodyDef.angle = leon::degToRad(rData.rotation);
	m_bodyDef.bullet = rData.isBullet;
	m_bodyDef.angularDamping = rData.angularDampening;
	m_bodyDef.linearDamping = rData.linearDampening;

	m_pBody = game.getUniverse().getWorld().CreateBody(&m_bodyDef);
	m_pBody->SetUserData(this);

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
	b2Body& rm_pBody = *m_pBody;
	m_pBody->SetAwake(true);
	bool awake = m_pBody->IsAwake();
	return awake;
}
void BodyComponent::sleep()
{
	///TODO WHY DOESN'T THIS WORK
	//if(isAwake())
	//{
	m_oldAngle = m_pBody->GetAngle();
	m_oldPos = m_pBody->GetPosition();

	m_pBody->SetActive(false);
	m_pBody->SetAwake(false);

	m_pBody->SetAngularVelocity(0);
	m_pBody->SetLinearVelocity(b2Vec2(0, 0));
	m_pBody->SetTransform(game.getUniverse().getBed(), 0);
	//}
	//else
	//	cout << FILELINE;
}
void BodyComponent::wake()
{
	//cout << "\nWake";
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
	//cout << "\nWake2";
	m_pBody->SetActive(true);
	m_pBody->SetAwake(true);
	m_pBody->SetTransform(rCoords, radiansCCW);
	m_pBody->SetLinearVelocity(rVel);
	m_pBody->SetAngularVelocity(angularVel);
}
void BodyComponent::setTeam(int team)
{
	m_team = team;
}
int BodyComponent::getTeam() const
{
	return m_team;
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


