#ifndef HEALTH_HPP
#define HEALTH_HPP

#include "Pool.hpp"

///Data constructor for Health
struct HealthData : public PoolData<int>
{
	HealthData() :
		PoolData(),
		Armor(0)
	{
		Max = 100;
		Value = 100;
		Min = 0;
	}

	int Armor;//how much damage is subtracted

	virtual void loadJson(const Json::Value& root);
};


//
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
