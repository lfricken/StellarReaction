#include "BallisticWeapon.hpp"
#include "Projectile.hpp"
#include "ProjectileMan.hpp"

using namespace std;

BallisticWeapon::BallisticWeapon(const BallisticWeaponData& rData) : Weapon(rData)
{
	m_projName = rData.projName;
	m_velocity = rData.velocity;
	m_projLifetime = rData.range / m_velocity;
}
BallisticWeapon::~BallisticWeapon()
{

}
void BallisticWeapon::preShot(const b2Vec2& center, const b2Vec2& aim, float radCCW)
{
	/**Fire projectiles**/
	Projectile* pProj = game.getUniverse().getProjMan().getProjectile(m_projName);
	sf::Packet damagePack;
	damagePack << m_damage;
	Message mes;
	float delay = 0.0f;
	bool replaceData = false;
	string command = "damage";
	string target = "";
	mes.reset(target, command, damagePack, delay, replaceData);

	b2Vec2 dif = aim - center;
	b2Vec2 vel = aim - center;
	vel.Normalize();
	vel *= m_velocity;
	pProj->launch(center, vel, atan2(dif.y, dif.x), 0, mes, m_projLifetime);///CHANGE FIRST 0 TO ANGLE of shot

}
void BallisticWeapon::postShot(const b2Vec2& center, const b2Vec2& aim, float radCCW)
{

}
