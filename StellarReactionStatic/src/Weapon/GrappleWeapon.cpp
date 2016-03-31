#include "GrappleWeapon.hpp"
#include "Convert.hpp"
#include "Universe.hpp"
#include "Globals.hpp"
#include "FixtureComponent.hpp"
#include "BlueprintLoader.hpp"
#include "Weapon.hpp"

using namespace std;


void GrappleWeaponData::loadJson(const Json::Value& root)
{
	GETJSON(pullStrength);
	GETJSON(pullTime);

	LaserWeaponData::loadJson(root);
}
GrappleWeapon::GrappleWeapon(const GrappleWeaponData& rData) : LaserWeapon(rData)
{
	m_grappleTimer.setCountDown(rData.pullTime);
	m_pullStrength = rData.pullStrength;
	m_target = NULL;
}
GrappleWeapon::~GrappleWeapon()
{

}
void GrappleWeapon::postPhysUpdate(const b2Vec2& center, const b2Vec2& aim, float32 radCCW, b2Body* pBody, float module_orientation)
{
	LaserWeapon::postPhysUpdate(center, aim, radCCW, pBody, module_orientation);
	if(!m_grappleTimer.isTimeUp())
		grappleTo();
}
void GrappleWeapon::grappleTo()
{
	if(m_target != NULL)
	{
		b2Vec2 targetPos = m_target->getBodyPtr()->GetPosition();
		b2Vec2 ourPos = m_pBody->GetPosition();
		b2Vec2 appliedForce = (targetPos - ourPos);

		appliedForce.Normalize();
		appliedForce *= m_pullStrength;
		m_pBody->ApplyForceToCenter(appliedForce, true);
		m_target->getBodyPtr()->ApplyForceToCenter(b2Vec2(-appliedForce.x, -appliedForce.y), true);
	}
}
Vec2 GrappleWeapon::collisionHandle(const RayData& data)
{
	m_target = static_cast<FixtureComponent*>(data.pFixture->GetUserData());
	m_grappleTimer.restartCountDown();
	return data.point;
}
