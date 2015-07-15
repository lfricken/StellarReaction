#include "LaserWeapon.hpp"
#include "Convert.hpp"
#include "Universe.hpp"
#include "Globals.hpp"

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
void LaserWeapon::preShot(const b2Vec2& center, const b2Vec2& aim, float radCCW)
{
	m_ray.setIgnoreBody(m_pBody);

	float mult = m_range/leon::Dist(aim, center);
	b2Vec2 end = b2Vec2(center.x+(aim.x-center.x)*mult, center.y+(aim.y-center.y)*mult);
	game.getUniverse().getWorld().RayCast(&m_ray, center, end);
}
void LaserWeapon::postShot(const b2Vec2& center, const b2Vec2& aim, float radCCW)
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
