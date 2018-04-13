#include "Health.hpp"
#include "stdafx.hpp"
#include "JSON.hpp"


void HealthData::loadJson(const Json::Value& root)
{
	RangeData::loadJson(root);

	GETJSON(Armor);

	GETJSON(PercentStartCrit);
	GETJSON(MaxCritOdds);
	GETJSON(MaxCrits);
}
Health::Health(const HealthData& data) : Range(data)
{
	RangeData armorData;
	armorData.Max = 10000;
	armorData.Value = (float)data.Armor;
	armorData.Min = 0;

	m_armor.reset(new Range(armorData));

	m_critHits = 0;
	m_maxCritHits = data.MaxCrits;

	m_percentStartCrit = data.PercentStartCrit;
	m_maxCritOdds = data.MaxCritOdds;
}
Health::~Health()
{

}
void Health::damage(int injure, bool* wasCrit, int* damageDealt, int* overkill)
{
	*damageDealt = injure - (int)m_armor->getValue();
	if(*damageDealt < 0)
		*damageDealt = 0;

	*overkill = *damageDealt - getHealth();
	if(*overkill < 0)
		*overkill = 0;

	*wasCrit = false;

	if(!isDead() && *damageDealt > 0)//we aren't dead, and are taking damage
	{
		changeValue((float)-*damageDealt);//deal damage
		changeArmor(-*damageDealt / 4);
		*wasCrit = updateCrits();
	}
}
bool Health::updateCrits()
{
	bool tookACrit = false;
	float health = getHealthPercent();
	if(!hasFullCrits())
	{
		if(health == 0.f)//full damage
		{
			m_critHits = m_maxCritHits;
			tookACrit = true;
		}
		else if(tryHitCrit())//not full damage, try crit hits
		{
			incrementCritHits();
			tookACrit = true;
		}
	}
	return tookACrit;
}
void Health::heal(int health)
{
	if(health > 0)
		changeValue((float)health);

	if(getHealthPercent() == 1.f)
		m_critHits = 0;
}
void Health::changeArmor(int change)
{
	m_armor->changeValue((float)change);
}

bool Health::isDead() const
{
	return (getValue() == getMin());
}
int Health::getHealth() const
{
	return (int)getValue();
}
int Health::getMaxHealth() const
{
	return (int)getMax();
}
float Health::getHealthPercent() const
{
	if(getMaxHealth() > 0)
		return (static_cast<float>(getHealth()) / static_cast<float>(getMaxHealth()));
	else
		return 0;
}
int Health::getArmor() const
{
	return (int)m_armor->getValue();
}
sf::Color Health::getColor(float percent)
{
	char g = static_cast<char>(255.f * percent);
	return sf::Color(255, g, 0);
}
sf::Color Health::getColor() const
{
	return getColor(getHealthPercent());
}
sf::Color Health::getCritColor() const
{
	return getColor(m_critHits / static_cast<float>(m_maxCritHits));
}
bool Health::trySelfCrit() const
{
	int rand = Rand::get(1, m_maxCritHits);//1,2,3
	return rand <= m_critHits;
}
bool Health::hasFullCrits() const
{
	return m_critHits == m_maxCritHits;
}
void Health::incrementCritHits()
{
	++m_critHits;
}
bool Health::tryHitCrit() const
{
	bool takeCrit = false;
	float health = getHealthPercent();
	if(health < m_percentStartCrit)
	{
		float subPer = (m_percentStartCrit - health) / m_percentStartCrit;//percent along crit line, remove zone where you can't take crits
		float critOdds = subPer * m_maxCritOdds;//output of function
		float roll = Rand::get(0.f, 1.f);

		if(roll < critOdds)
			takeCrit = true;
	}
	return takeCrit;
}
bool Health::hasCrits() const
{
	return m_critHits != 0;
}

