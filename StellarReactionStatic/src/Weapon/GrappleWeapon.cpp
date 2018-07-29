#include "GrappleWeapon.hpp"
#include "Convert.hpp"
#include "Universe.hpp"
#include "Globals.hpp"
#include "FixtureComponent.hpp"
#include "BlueprintLoader.hpp"
#include "Weapon.hpp"
#include "BodyComponent.hpp"

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
	m_target = nullptr;
}
GrappleWeapon::~GrappleWeapon()
{

}
void GrappleWeapon::postPhysUpdate(const Vec2& center, const Vec2& aim, float32 radCCW, BodyComponent* pBody, float module_orientation)
{
	//LaserWeapon::postPhysUpdate(center, aim, radCCW, pBody, module_orientation);
	//if(!m_grappleTimer.isTimeUp())
	//	grappleTo();
}
void GrappleWeapon::grappleTo()
{
	if(m_target != nullptr)
	{
		Vec2 targetPos = m_target->getCenter();
		Vec2 ourPos = m_pParentBody->getPosition();
		Vec2 appliedForce = (targetPos - ourPos);

		appliedForce = appliedForce.unit();
		appliedForce *= m_pullStrength;
		m_pParentBody->applyForce(appliedForce);
		m_target->getParentBody()->applyForce(appliedForce.inv());
	}
}
Vec2 GrappleWeapon::collisionHandle(const RayData& data)
{
	m_target = static_cast<FixtureComponent*>(data.pFixture->GetUserData());
	m_grappleTimer.restartCountDown();
	return data.point;
}
