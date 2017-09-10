#include "Health.hpp"
#include "stdafx.hpp"


void HealthData::loadJson(const Json::Value& root)
{
	PoolData::loadJson<int>(root);

	GETJSON(Armor);
	GETJSON(PercentStartCrit);
	GETJSON(MaxCritOdds);
}
Health::Health(const HealthData& rData) : Pool(rData)
{
	m_armor = rData.Armor;
	m_critHits = 0;

	m_percentStartCrit = rData.PercentStartCrit;
	m_maxCritOdds = rData.MaxCritOdds;
}
Health::~Health()
{

}
bool Health::damage(int injure)
{
	bool crit = false;
	int damageDealt = injure - m_armor;


	if(damageDealt > 0)
	{
		changeValue(-damageDealt);//deal damage
		m_armor -= damageDealt / 4;

		//determine whether a crit hit
		{
			float health = getHealthPercent();
			if(health > m_percentStartCrit && m_critHits < 3)
			{
				if(getHealthPercent() == 0.f)//full damage
				{
					m_critHits = 3;
					crit = true;

					put logic into function, remove armor every 3rd damage dealt from 4?
				} 
				else//not full damage, try crit hits
				{
					float len = 1.f - m_percentStartCrit;
					float subPer = health / len;//percent along crit line
					float critOdds = subPer * m_maxCritOdds;//output of function
					float roll = Rand::get(0.f, 1.f);

					if(roll < critOdds)
					{
						crit = true;
					}
				}
			}
			else
				crit = false;
		}
	}
	return crit;
}

void Health::heal(int health)
{
	if(health > 0)
		changeValue(health);
	
	if(getHealthPercent() == 1.f)
		m_critHits = 0;
}
void Health::changeArmor(int change)
{
	m_armor += change;
}

bool Health::isDead() const
{
	return (getValue() == getMin());
}
int Health::getHealth() const
{
	return getValue();
}
int Health::getMaxHealth() const
{
	return getMax();
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
	return m_armor;
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
	return getColor(m_critHits / 3.f);
}
bool Health::tryOperationCrit() const
{
	auto rand = Rand::get(1, 4);//1,2,3
	return rand <= m_critHits;
}
bool Health::hasFullCrits() const
{
	return m_critHits == 3;
}
void Health::incrementCritHits()
{
	++m_critHits;
}