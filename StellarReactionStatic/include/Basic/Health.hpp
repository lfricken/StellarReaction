#ifndef HEALTH_HPP
#define HEALTH_HPP

#include "Range.hpp"

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
struct HealthData : public RangeData
{
	HealthData() :
		RangeData(),
		Armor(0)
	{
		Max = 100;
		Value = 100;
		Min = 0;

		PercentStartCrit = 0.7f;
		MaxCritOdds = 0.3f;
		MaxCrits = 3;
	}

	int Armor; // How much damage is subtracted per hit.

	float PercentStartCrit; // At what percentage does the odds of a crit go above 0.
	float MaxCritOdds; // At 100% damage, what are the odds of a crit. 
	int MaxCrits; // Number of crits for this object to stop working.

	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};


///Manages the health of a Module.
class Health : private Range
{
public:
	Health(const HealthData& rData);
	virtual ~Health();

	/// <summary>
	/// Deal damage to us.
	/// </summary>
	void damage(int injure, bool* wasCrit, int* damageDealt, int* overkill);
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
	/// Performs a crit roll. Returns result.
	/// </summary>
	/// <returns></returns>
	bool tryHitCrit() const;
	/// <summary>
	/// Return true if this should get a crit for operating.
	/// </summary>
	bool trySelfCrit() const;
	/// <summary>
	/// Returns true if this should stop operating.
	/// </summary>
	bool hasFullCrits() const;
	/// <summary>
	/// True if we have some critical damage.
	/// </summary>
	bool hasCrits() const;
	/// <summary>
	/// Increase crit hits by one.
	/// </summary>
	void incrementCritHits();

protected:
	/// <summary>
	/// Returns true if we increased crits. Assumes we just took damage.
	/// </summary>
	bool updateCrits();
private:

	///Armor reduces incoming damage by that amount. Can't cause negative damage.
	sptr<Range> m_armor;

	int m_critHits;
	int m_maxCritHits;
	float m_percentStartCrit;
	float m_maxCritOdds;
};

#endif // HEALTH_HPP
