#ifndef HEALTH_HPP
#define HEALTH_HPP

#include "Pool.hpp"

struct HealthData : public PoolData<int>
{
	HealthData() :
		PoolData(),
		armor(0)
	{
		startMax = 1000;
		startValue = 1000;
		startMin = 0;
	}

	int armor;//how much damage is subtracted
};

class Health : private Pool<int>
{
public:
	Health(const HealthData& rData);
	virtual ~Health();

	void damage(int injure);
	void heal(int health);
	void changeArmor(int change);

	bool isDead() const;
	int getHealth() const;
	int getMaxHealth() const;
	float getHealthPercent() const;
	int getArmor() const;


protected:
private:

	int m_armor;
};

#endif // HEALTH_HPP
