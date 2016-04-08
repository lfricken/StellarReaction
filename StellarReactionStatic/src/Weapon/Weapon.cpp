#include "Weapon.hpp"
#include "FixtureComponent.hpp"
#include "Message.hpp"
#include "Universe.hpp"
#include "IOManager.hpp"
#include "SoundManager.hpp"
#include "Player.hpp"

using namespace std;

void WeaponData::loadJson(const Json::Value& root)
{
	GETJSON(energyConsumption);
	GETJSON(ballisticConsumption);
	GETJSON(missileConsumption);
	GETJSON(shots);
	GETJSON(shotsInSpread);
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
void Weapon::prePhysUpdate(const b2Vec2& center, const b2Vec2& aim, float32 radCCW, b2Body* pBody, float module_orientation)//we are checking whether we should take a shot
{
	m_pBody = pBody;

	if(m_shotsRemain>0 && m_shotTimer.isTimeUp())
	{
		--m_shotsRemain;
		m_shotSound.pos = center;
		game.getSound().playSound(m_shotSound);
		m_shotTimer.restartCountDown();
		m_shotThisTick = true;

		if (m_shotsInSpread == 1)
			preShot(center, aim, radCCW, module_orientation);
		else {
			int i;
			for (i = 0; i < m_shotsInSpread; i++) {
				float new_y = ((float)rand()) / RAND_MAX;
				float new_x = (-1 * aim.y * new_y) / aim.x;
				b2Vec2 perp(new_x, new_y);
				b2Vec2 newAim = aim + perp;
				preShot(center, newAim, radCCW, module_orientation);
			}
		}

	}
}
void Weapon::postPhysUpdate(const b2Vec2& center, const b2Vec2& aim, float32 radCCW, b2Body* pBody, float module_orientation)//we are determining our next shot
{
	m_pBody = pBody;
	m_decor.setRotation(radCCW);
	m_decor.setPosition(center);

	if(m_shotThisTick)
	{
		m_shotThisTick = false;

		if (m_shotsInSpread == 1)
			postShot(center, aim, radCCW, module_orientation);
		else {
			int i;
			for (i = 0; i < m_shotsInSpread; i++) {
				float new_y = ((float)rand()) / RAND_MAX;
				float new_x = (-1 * aim.y * new_y) / aim.x;
				b2Vec2 perp(new_x, new_y);
				b2Vec2 newAim = aim + perp;
				postShot(center, newAim, radCCW, module_orientation);
			}
		}

		if(m_shotsRemain == 0)
		{
			m_endSound.pos = center;
			game.getSound().playSound(m_endSound);
		}
	}
}
void Weapon::damage(IOManager* pMessageReciever, int ioTargetPos, int damageAmount, int ioCausePos)
{
	int team = game.getLocalPlayer().getTeam();
	sf::Packet packet;
	packet << damageAmount << ioCausePos << team;

	Message mess;
	mess.reset(ioTargetPos, "damage", packet, 0.f, false);

	pMessageReciever->recieve(mess);
}
void Weapon::damage(b2Fixture* pFix, int damage)
{
	FixtureComponent& rComp = *static_cast<FixtureComponent*>(pFix->GetUserData());
	Weapon::damage(&game.getUniverse().getUniverseIO(), rComp.getIOPos(), (m_damage / m_shots), m_pTempParent->getIOPos());
}
QuadComponent* Weapon::getDecor()
{
	return &m_decor;
}


