#ifndef HEALTH_HPP
#define HEALTH_HPP

#include "Pool.hpp"

/// <summary>
/// Used to represent whether a module can be functioning or not.
/// </summary>
enum class HealthState
{
	Nominal,//the module is healthy and works
	CriticallyDamaged,//the module has taken critical damage and might not work
	Broken,//the module is severely damaged and definitely does not work
};


///Data constructor for Health.
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

	int Armor;///How much damage is subtracted per hit.

	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};


///Manages the health of a Module.
class Health : private Pool<int>
{
public:
	Health(const HealthData& rData);
	virtual ~Health();

	///Deal damage to us.
	void damage(int injure);
	///Restore health.
	void heal(int health);
	///Changes the armor by the specfied amount. Defaults to addition rather than subraction.
	void changeArmor(int change);

	///Do we have min health (usually 0).
	bool isDead() const;
	///Current health.
	int getHealth() const;
	///Max health.
	int getMaxHealth() const;
	///Percentage ,0-1, of our health. current/max.
	float getHealthPercent() const;
	///Current value of armor.
	int getArmor() const;


protected:
private:

	///Armor reduces incoming damage by that amount. Can't cause negative damage.
	int m_armor;
};

#endif // HEALTH_HPP
