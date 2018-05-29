#include "LaserWeapon.hpp"
#include "Convert.hpp"
#include "Universe.hpp"
#include "Globals.hpp"
#include "FixtureComponent.hpp"
#include "BlueprintLoader.hpp"
#include "Player.hpp"
#include "BodyComponent.hpp"

void LaserWeaponData::loadJson(const Json::Value& root)
{
	WeaponData::loadJson(root);

	GETJSON(beamWidth);
	GETJSON(beamColor);
	GETJSON(showTime);
	LOADJSON(beamComp.start);
	LOADJSON(beamComp.end);
	LOADJSON(beamComp);

}
LaserWeapon::LaserWeapon(const LaserWeaponData& rData) : Weapon(rData), m_beam(rData.beamComp)
{
	m_beamColor = rData.beamColor;
	m_beamWidth = rData.beamWidth;
	m_showTime = rData.showTime;
	m_ray.addMask(Mask::Laser);
}
LaserWeapon::~LaserWeapon()
{

}
void LaserWeapon::preShot(const Vec2& center, const Vec2& aim, float radCCW, float module_orientation)
{
	m_pBody->setIgnoreBody(&m_ray);
	
	const Vec2 angle = center.to(aim).unit();
	const Vec2 laserVector = angle * m_range;
	const Vec2 end = aim + laserVector;
	game.getUniverse().getWorld().RayCast(&m_ray, Convert::universeToWorld(center), Convert::universeToWorld(end));
}
void LaserWeapon::postShot(const Vec2& fixtureDoingDamageCenter, const Vec2& aim, float radCCW, float module_orientation)
{
	const Map<float, RayData>& collisions = m_ray.getLatest();

	Vec2 end;
	const Vec2 shotDir = fixtureDoingDamageCenter.to(aim);
	if(collisions.empty())
	{
		float mult = m_range / aim.to(fixtureDoingDamageCenter).len();
		end = fixtureDoingDamageCenter + shotDir * mult;
	}
	else
	{
		int i = 0;
		for(auto it = collisions.cbegin(); i < m_collisions && it != collisions.cend(); ++it, ++i)
		{
			const Vec2 hit = it->second.point;
			damage(it->second.pFixture, m_damage, hit, shotDir);
			end = hit;
		}
	}

	m_beam.setStart(fixtureDoingDamageCenter);
	m_beam.setEnd(end);
	m_beam.activate(m_showTime, m_beamWidth, m_beamColor);

	m_ray.reset();
}
