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

		PercentStartCrit = 0.3f;
		MaxCritOdds = 0.3f;
	}

	int Armor;///How much damage is subtracted per hit.
	/// <summary>
	/// At what percentage does the odds of a crit go above 0.
	/// </summary>
	float PercentStartCrit;
	/// <summary>
	/// At 100% damage, what are the odds of a crit. 
	/// </summary>
	float MaxCritOdds;

	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};


///Manages the health of a Module.
class Health : private Pool<int>
{
public:
	Health(const HealthData& rData);
	virtual ~Health();

	/// <summary>
	/// Deal damage to us. Return true if a crit damage.
	/// </summary>
	bool damage(int injure);
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
	/// <summary>
	/// Returns the color for a health percentage.
	/// </summary>
	static sf::Color getColor(float percent);
	/// <summary>
	/// Returns the color for a health percentage.
	/// </summary>
	sf::Color getColor() const;

	/// <summary>
	/// Returns the color for the crit percentage.
	/// </summary>
	sf::Color getCritColor() const;
	/// <summary>
	/// Return true if this should get a crit for operating.
	/// </summary>
	bool tryOperationCrit() const;
	/// <summary>
	/// Returns true if this should stop operating.
	/// </summary>
	bool hasFullCrits() const;

	void incrementCritHits();
protected:
private:

	///Armor reduces incoming damage by that amount. Can't cause negative damage.
	int m_armor;

	int m_critHits;
	float m_percentStartCrit;
	float m_maxCritOdds;
};

#endif // HEALTH_HPP
