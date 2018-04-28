


enum class UpgradeType
{
	Armor,
	Health,
	Other, // Damage, Energy Gen, Brain Power, Sensor Power, Thrust Force, Storage.
};

/// <summary>
/// Constants related to upgrading things.
/// </summary>
class Upgrade
{
public:
	/// <summary>
	/// Upgrades the value.
	/// </summary>
	static void increase(UpgradeType type, float* value, float initValue, int level);
	/// <summary>
	/// Upgrades the value.
	/// </summary>
	static void increase(UpgradeType type, int* value, int initValue, int level);

private:
	static float getAmount(int level);
};


