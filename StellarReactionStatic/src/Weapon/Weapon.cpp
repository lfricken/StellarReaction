#include "Weapon.hpp"
#include "FixtureComponent.hpp"
#include "Message.hpp"
#include "Universe.hpp"
#include "IOManager.hpp"
#include "SoundManager.hpp"
#include "Player.hpp"

#include "Convert.hpp"
#include "Team.hpp"
#include "UpgradeType.hpp"
#include "BodyComponent.hpp"

void WeaponData::applyUpgrade(UpgradeType type)
{
	BlueprintableData::applyUpgrade(type);
	Upgrade::increase(type, &damage, startDamage, upgradeLevel);
}
void WeaponData::loadJson(const Json::Value& root)
{
	GETJSON(energyConsumption);
	GETJSON(ballisticConsumption);
	GETJSON(missileConsumption);
	GETJSON(shots);
	GETJSON(shotsInSpread);
	GETJSON(fireArc);
	GETJSON(damage);
	startDamage = damage;

	LOADJSON(startSound);
	LOADJSON(shotSound);
	LOADJSON(endSound);

	GETJSON(shotDelay);
	GETJSON(fireDelay);
	GETJSON(range);
	GETJSON(collisions);
	GETJSON(effectName);

	LOADJSON(weaponQuad);
}
Weapon::Weapon(const WeaponData& rData) : Blueprintable(rData), m_decor(rData.weaponQuad)
{
	m_effectName = rData.effectName;

	m_startSound = rData.startSound;
	m_shotSound = rData.shotSound;
	m_endSound = rData.endSound;

	m_energy = rData.energyConsumption;
	m_ballistic = rData.ballisticConsumption;
	m_missiles = rData.missileConsumption;

	m_range = rData.range;
	m_damagePerShot = rData.damage / rData.shots;
	m_shots = rData.shots;
	m_shotsInSpread = rData.shotsInSpread;
	m_fireArc = Convert::degToRad(rData.fireArc);
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
bool Weapon::fire(const FixtureComponent& pParent, RangeList* ranges)//we were told to fire
{

	if(m_fireTimer.isTimeUp() && (*ranges)[RangeList::Energy].getValue() >= m_energy && (*ranges)[RangeList::Ballistic].getValue() >= m_ballistic && (*ranges)[RangeList::Missiles].getValue() >= m_missiles)
	{
		m_pTempParent = &pParent;
		m_startSound.play(m_decor.getPosition());
		m_decor.getAnimator().setAnimation("Fire", m_fireDelay);
		m_fireTimer.restartCountDown();
		(*ranges)[RangeList::Energy].changeValue(-m_energy);
		(*ranges)[RangeList::Ballistic].changeValue(-m_ballistic);
		(*ranges)[RangeList::Missiles].changeValue(-m_missiles);
		m_shotsRemain = m_shots;

		return true;
	}
	else
		return false;
}
void Weapon::prePhysUpdate(const Vec2& center, const Vec2& aim, BodyComponent* pBody, float functionalCapacity)//we are checking whether we should take a shot
{
	if(m_shotsRemain > 0 && m_shotTimer.isTimeUp())
	{
		--m_shotsRemain;
		m_shotTimer.restartCountDown();

		if(Rand::didSucceed(functionalCapacity))
		{
			m_shotThisTick = true;
			m_shotSound.play(m_decor.getPosition());
			for(int i = 0; i < m_shotsInSpread; i++)
			{
				ShotData data;
				data.aim = randArc(center, aim);
				data.shipRotation = pBody->getAngle();
				data.weaponCenter = center;

				preShot(data);
			}
		}
	}
}
void Weapon::postPhysUpdate(const Vec2& center, const Vec2& aim, BodyComponent* pBody)//we are determining our next shot
{
	m_decor.setRotation(center.to(aim).toAngle());
	m_decor.setPosition(center);

	if(m_shotThisTick)
	{
		m_shotThisTick = false;

		for(int i = 0; i < m_shotsInSpread; i++)
		{
			ShotData data;
			data.aim = randArc(center, aim);
			data.shipRotation = pBody->getAngle();
			data.weaponCenter = center;

			preShot(data);
		}
	}

	if(m_shotsRemain == 0)
		m_endSound.play(center);
}
void Weapon::damage(IOManager* pMessageReciever, int ioTargetPos, int damageAmount, int ioCausePos, Team team, const Vec2& collisionPoint, const Vec2& fromDirection, const String& effect, bool bleed)
{
	sf::Packet packet;
	packet << damageAmount << ioCausePos << static_cast<int>(team) << collisionPoint.x << collisionPoint.y << fromDirection.x << fromDirection.y << effect << bleed;

	Message mess;
	mess.reset(ioTargetPos, "damage", packet, 0.f, false);
	pMessageReciever->recieve(mess);
}
void Weapon::damage(b2Fixture* pFixtureTakingDamage, int damageAmount, const Vec2& collisionPoint, const Vec2& fromDirection)
{
	FixtureComponent& rComp = *static_cast<FixtureComponent*>(pFixtureTakingDamage->GetUserData());
	Weapon::damage(&getGame()->getUniverse().getUniverseIO(), rComp.getIOPos(), (damageAmount / m_shots), m_pTempParent->getIOPos(), m_team, collisionPoint, fromDirection, m_effectName, false);
}
QuadComponent* Weapon::getDecor()
{
	return &m_decor;
}
void Weapon::setParentData(BodyComponent* parent)
{
	m_pParentBody = parent;
	m_team = m_pParentBody->getTeam();
}
Vec2 Weapon::randArc(const Vec2& center, const Vec2& aim) const
{
	auto relativeVec = center.to(aim);
	return relativeVec.rotate(Rand::get(-m_fireArc, m_fireArc)) + center;
}
