#include "Health.hpp"
#include "stdafx.hpp"
using namespace std;

Health::Health(const HealthData& rData) : Pool(rData)
{
	m_armor = rData.armor;
}
Health::~Health()
{

}
void Health::damage(int injure)
{
	if(injure > m_armor)
		changeValue(-(injure-m_armor));
}
void Health::heal(int health)
{
	if(health > 0)
		changeValue(health);
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
		return (static_cast<float>(getHealth())/static_cast<float>(getMaxHealth()));
	else
		return 0;
}
int Health::getArmor() const
{
	return m_armor;
}
