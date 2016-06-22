#include "LaserWeapon.hpp"
#include "Convert.hpp"
#include "Universe.hpp"
#include "Globals.hpp"
#include "FixtureComponent.hpp"
#include "BlueprintLoader.hpp"
#include "Player.hpp"

using namespace std;


void LaserWeaponData::loadJson(const Json::Value& root)
{
	WeaponData::loadJson(root);

	GETJSON(beamWidth);
	if(!root["beamColor"].isNull())
		beamColor = BlueprintLoader::loadColor(root["beamColor"]);
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
/// <summary>
/// Casts the raycast to get colliding bodies
/// </summary>
/// <param name="center">The center.</param>
/// <param name="aim">The aim.</param>
/// <param name="radCCW">The RAD CCW.</param>
void LaserWeapon::preShot(const b2Vec2& center, const b2Vec2& aim, float radCCW, float module_orientation)
{
	m_ray.setIgnoreBody(m_pBody);

	float mult = m_range / leon::Dist(aim, center);
	b2Vec2 end = b2Vec2(center.x + (aim.x - center.x)*mult, center.y + (aim.y - center.y)*mult);
	game.getUniverse().getWorld().RayCast(&m_ray, center, end);
}
/// <summary>
/// after physics calculation, we know who we hit, so we send a damage packet
/// </summary>
/// <param name="center">The center.</param>
/// <param name="aim">The aim.</param>
/// <param name="radCCW">The RAD CCW.</param>
void LaserWeapon::postShot(const b2Vec2& center, const b2Vec2& aim, float radCCW, float module_orientation)
{
	const Map<float, RayData>& collisions = m_ray.getLatest();

	b2Vec2 end;

	if(collisions.empty())
	{
		float mult = m_range / leon::Dist(aim, center);
		end = b2Vec2(center.x + (aim.x - center.x)*mult, center.y + (aim.y - center.y)*mult);
	}
	else
	{
		int i = 0;
		for(auto it = collisions.cbegin(); i < m_collisions && it != collisions.cend(); ++it, ++i)
		{
			end = collisionHandle(it->second);
		}
	}

	m_beam.setStart(center);
	m_beam.setEnd(end);
	m_beam.activate(m_showTime, m_beamWidth, m_beamColor);

	m_ray.reset();
}
Vec2 LaserWeapon::collisionHandle(const RayData& data)
{
	damage(data.pFixture, m_damage);
	return data.point;
}
