#include "LaserWeapon.hpp"
#include "Convert.hpp"
#include "Universe.hpp"
#include "Globals.hpp"
#include "FixtureComponent.hpp"
#include "BlueprintLoader.hpp"

using namespace std;

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

	float mult = m_range/leon::Dist(aim, center);
	b2Vec2 end = b2Vec2(center.x+(aim.x-center.x)*mult, center.y+(aim.y-center.y)*mult);
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
	RayData data = m_ray.getLatest();
	m_ray.reset();
	b2Vec2 end;

	if(data.pFixture != NULL)
	{
		end = data.point;
		damage(data.pFixture, m_damage);
	}
	else
	{
		float mult = m_range/leon::Dist(aim, center);
		end = b2Vec2(center.x+(aim.x-center.x)*mult, center.y+(aim.y-center.y)*mult);
	}

	m_beam.setStart(center);
	m_beam.setEnd(end);
	m_beam.activate(m_showTime, m_beamWidth, m_beamColor);
}

void LaserWeaponData::loadJson(const Json::Value& root)
{
	WeaponData::loadJson(root);

	if(!root["BeamWidth"].isNull())
		beamWidth = root["BeamWidth"].asInt();
	if(!root["BeamColor"].isNull())
		beamColor = BlueprintLoader::loadColor(root["BeamColor"]);
	if(!root["ShowTime"].isNull())
		showTime = root["ShowTime"].asFloat();
	if(!root["BeamStart"].isNull())
		beamComp.loadJson(root["BeamStart"]);
	if(!root["BeamEnd"].isNull())
		beamComp.loadJson(root["BeamEnd"]);
	if(!root["BeamMid"].isNull())
		static_cast<QuadComponentData>(beamComp).loadJson(root["BeamMid"]);

}