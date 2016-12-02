#include "Weapon.hpp"
#include "FixtureComponent.hpp"
#include "Message.hpp"
#include "Universe.hpp"
#include "IOManager.hpp"
#include "SoundManager.hpp"
#include "Player.hpp"

#include "Convert.hpp"
#include "Team.hpp"

using namespace std;

void WeaponData::loadJson(const Json::Value& root)
{
	GETJSON(energyConsumption);
	GETJSON(ballisticConsumption);
	GETJSON(missileConsumption);
	GETJSON(shots);
	GETJSON(shotsInSpread);
	GETJSON(fireArc);
	GETJSON(damage);

	LOADJSON(startSound);
	LOADJSON(shotSound);
	LOADJSON(endSound);

	GETJSON(shotDelay);
	GETJSON(fireDelay);
	GETJSON(range);
	GETJSON(collisions);

	LOADJSON(weaponQuad);
}
Weapon::Weapon(const WeaponData& rData) : m_decor(rData.weaponQuad)
{
	m_startSound = rData.startSound;
	m_shotSound = rData.shotSound;
	m_endSound = rData.endSound;

	m_energy = rData.energyConsumption;
	m_ballistic = rData.ballisticConsumption;
	m_missiles = rData.missileConsumption;

	m_range = rData.range * sizeScalingFactor;
	m_damage = rData.damage;
	m_shots = rData.shots;
	m_shotsInSpread = rData.shotsInSpread;
	m_fireArc = leon::degToRad(rData.fireArc);
	m_shotsRemain = 0;
	m_collisions = rData.collisions;

	m_fireTimer.setCountDown(rData.fireDelay);
	m_fireTimer.restartCountDown();
	m_fireDelay = rData.fireDelay;
	m_shotTimer.setCountDown(rData.shotDelay);
	m_shotTimer.restartCountDown();

	m_shotThisTick = false;

	m_team = Team::Invalid;
}
Weapon::~Weapon()
{

}
bool Weapon::fire(const FixtureComponent& pParent, Pool<Energy>* pEnergy, Pool<Ballistic>* pBall, Pool<Missiles>* pMis)//we were told to fire
{

	if(m_fireTimer.isTimeUp() && pEnergy->getValue() >= m_energy && pBall->getValue() >= m_ballistic && pMis->getValue() >= m_missiles)
	{
		m_pTempParent = &pParent;
		m_startSound.play(m_decor.getPosition());
		m_decor.getAnimator().setAnimation("Fire", m_fireDelay);
		m_fireTimer.restartCountDown();
		pEnergy->changeValue(-m_energy);
		pBall->changeValue(-m_ballistic);
		pMis->changeValue(-m_missiles);
		m_shotsRemain = m_shots;

		return true;
	}
	else
		return false;
}
void Weapon::prePhysUpdate(const Vec2& center, const Vec2& aim, float32 radCCW, b2Body* pBody, float module_orientation)//we are checking whether we should take a shot
{
	m_pBody = pBody;

	if(m_shotsRemain > 0 && m_shotTimer.isTimeUp())
	{
		--m_shotsRemain;
		m_shotSound.play(m_decor.getPosition());
		m_shotTimer.restartCountDown();
		m_shotThisTick = true;

		for(int i = 0; i < m_shotsInSpread; i++)
			preShot(center, randArc(center, aim), radCCW, module_orientation);
	}
}
void Weapon::postPhysUpdate(const Vec2& center, const Vec2& aim, float32 radCCW, b2Body* pBody, float module_orientation)//we are determining our next shot
{
	m_pBody = pBody;
	m_decor.setRotation(radCCW);
	m_decor.setPosition(center);

	if(m_shotThisTick)
	{
		m_shotThisTick = false;

		for(int i = 0; i < m_shotsInSpread; i++)
			postShot(center, randArc(center, aim), radCCW, module_orientation);
	}

	if(m_shotsRemain == 0)
		m_endSound.play(center);
}
void Weapon::damage(IOManager* pMessageReciever, int ioTargetPos, int damageAmount, int ioCausePos, Team team)
{
	sf::Packet packet;
	packet << damageAmount << ioCausePos << static_cast<int>(team);

	Message mess;
	mess.reset(ioTargetPos, "damage", packet, 0.f, false);

	pMessageReciever->recieve(mess);
}
void Weapon::damage(b2Fixture* pFix, int damage)
{
	FixtureComponent& rComp = *static_cast<FixtureComponent*>(pFix->GetUserData());
	Weapon::damage(&game.getUniverse().getUniverseIO(), rComp.getIOPos(), (m_damage / m_shots), m_pTempParent->getIOPos(), m_team);
}
QuadComponent* Weapon::getDecor()
{
	return &m_decor;
}
void Weapon::setTeam(Team newTeam)
{
	m_team = newTeam;
}
Vec2 Weapon::randArc(const Vec2& center, const Vec2& aim) const
{
	auto relativeVec = center.to(aim);
	return relativeVec.rotate(Rand::get(-m_fireArc, m_fireArc)) + center;
}
