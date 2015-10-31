#include "Weapon.hpp"
#include "FixtureComponent.hpp"
#include "Message.hpp"
#include "Universe.hpp"
#include "IOManager.hpp"
#include "SoundManager.hpp"

using namespace std;

Weapon::Weapon(const WeaponData& rData) : m_decor(rData.weaponQuad)
{
	m_startSound = rData.startSound;
	m_shotSound = rData.shotSound;
	m_endSound = rData.endSound;

	m_energy = rData.ener;
	m_ballistic = rData.ball;
	m_missiles = rData.mis;

	m_range = rData.range * sizeScalingFactor;
	m_damage = rData.damage;
	m_shots = rData.shots;
	m_shotsRemain = 0;
	m_collisions = rData.collisions;

	m_fireTimer.setCountDown(rData.fireDelay);
	m_fireTimer.restartCountDown();
	m_fireDelay = rData.fireDelay;
	m_shotTimer.setCountDown(rData.shotDelay);
	m_shotTimer.restartCountDown();

	m_shotThisTick = false;
}
Weapon::~Weapon()
{

}
bool Weapon::fire(const FixtureComponent& pParent, Pool<Energy>* pEnergy, Pool<Ballistic>* pBall, Pool<Missiles>* pMis)//we were told to fire
{

	if(m_fireTimer.isTimeUp() && pEnergy->getValue()>=m_energy && pBall->getValue()>=m_ballistic && pMis->getValue()>=m_missiles)
	{
		m_pTempParent = &pParent;
		m_startSound.pos = m_decor.getPosition();
		game.getSound().playSound(m_startSound);
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
void Weapon::prePhysUpdate(const b2Vec2& center, const b2Vec2& aim, float32 radCCW, b2Body* pBody)//we are checking whether we should take a shot
{
	m_pBody = pBody;

	if(m_shotsRemain>0 && m_shotTimer.isTimeUp())
	{
		--m_shotsRemain;
		m_shotSound.pos = center;
		game.getSound().playSound(m_shotSound);
		m_shotTimer.restartCountDown();
		m_shotThisTick = true;

		preShot(center, aim, radCCW);
	}
}
void Weapon::postPhysUpdate(const b2Vec2& center, const b2Vec2& aim, float32 radCCW, b2Body* pBody)//we are determining our next shot
{
	m_pBody = pBody;
	m_decor.setRotation(radCCW);
	m_decor.setPosition(center);

	if(m_shotThisTick)
	{
		m_shotThisTick = false;
		postShot(center, aim, radCCW);

		if(m_shotsRemain == 0)
		{
			m_endSound.pos = center;
			game.getSound().playSound(m_endSound);
		}
	}
}
void Weapon::damage(b2Fixture* pFix, int damage)
{
	FixtureComponent& rComp = *static_cast<FixtureComponent*>(pFix->GetUserData());
	sf::Packet packet;
	packet << (m_damage / m_shots);
	Message mess;
	mess.reset(rComp.getIOPos(), "damage", packet, 0.f, false);
	game.getUniverse().getUniverseIO().recieve(mess);
}