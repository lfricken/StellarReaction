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
	LaserWeaponData::loadJson(root);
}
GrappleWeapon::GrappleWeapon(const GrappleWeaponData& rData) : LaserWeapon(rData)
{
	m_time.setCountDown(3.0);
	target = NULL;
}
GrappleWeapon::~GrappleWeapon()
{

}
/// <summary>
/// Casts the raycast to get colliding bodies
/// </summary>
/// <param name="center">The center.</param>
/// <param name="aim">The aim.</param>
/// <param name="radCCW">The RAD CCW.</param>
void GrappleWeapon::preShot(const b2Vec2& center, const b2Vec2& aim, float radCCW, float module_orientation)
{
	LaserWeapon::preShot(center, aim, radCCW, module_orientation);
}
/// <summary>
/// after physics calculation, we know who we hit, so we send a damage packet
/// </summary>
/// <param name="center">The center.</param>
/// <param name="aim">The aim.</param>
/// <param name="radCCW">The RAD CCW.</param>
void GrappleWeapon::postShot(const b2Vec2& center, const b2Vec2& aim, float radCCW, float module_orientation)
{
	RayData data = m_ray.getLatest();
	m_ray.reset();
	b2Vec2 end;

	if (data.pFixture != NULL)
	{
		end = data.point;
		target = static_cast<FixtureComponent*>(data.pFixture->GetUserData());
		m_time.restartCountDown();
	}
	else
	{
		float mult = m_range / leon::Dist(aim, center);
		end = b2Vec2(center.x + (aim.x - center.x)*mult, center.y + (aim.y - center.y)*mult);
	}

	m_beam.setStart(center);
	m_beam.setEnd(end);
	m_beam.activate(m_showTime, m_beamWidth, m_beamColor);
}
void GrappleWeapon::postPhysUpdate(const b2Vec2& center, const b2Vec2& aim, float32 radCCW, b2Body* pBody, float module_orientation)
{
	LaserWeapon::postPhysUpdate(center, aim, radCCW, pBody, module_orientation);
	if (!m_time.isTimeUp())
		grappleTo();
}
void GrappleWeapon::grappleTo()
{
	if (target != NULL)
	{
		b2Vec2 targetPos = target->getBodyPtr()->GetPosition();
		// move our ship to targetPos
		b2Vec2 ourPos = m_pBody->GetPosition();
		b2Vec2 appliedForce = (targetPos - ourPos);
		float mag = sqrt(pow((targetPos.x - ourPos.x), 2) + pow((targetPos.y - ourPos.y), 2));
		appliedForce.Normalize();
		appliedForce *= mag * 5;
		m_pBody->ApplyForceToCenter(appliedForce, true);
		target->getBodyPtr()->ApplyForceToCenter(b2Vec2(-appliedForce.x, -appliedForce.y), true);
	}
}