#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "stdafx.hpp"
#include "RangeList.hpp"
#include "QuadComponent.hpp"
#include "Globals.hpp"
#include "ClassRegister.hpp"
#include "Blueprintable.hpp"
#include "NonCopyable.hpp"
#include "Sound.hpp"
#include "Debugging.hpp"

enum class Team;
struct WeaponData;
class FixtureComponent;
enum class UpgradeType;
class BodyComponent;

struct HitEffect
{
	String effect;
	Vec2 pos;
	Vec2 normal;
};

/// Basic object that provides easier interface for making complex types of weapons.
class Weapon : public Blueprintable, NonCopyable
{
public:
	struct ShotData
	{
		Vec2 weaponCenter;
		Vec2 aim;
		float shipRotation;
	};

	Weapon(const WeaponData& rData);
	virtual ~Weapon();
	/// Called when the controller wants to fire. This activates and consumes any ammo,
	/// and sets the weapon so it will fire on preShot and postShot.
	/// returns true if we will fire.
	bool fire(const FixtureComponent& pParent, RangeList* ranges);
	/// Called by our parent module
	virtual void prePhysUpdate(const Vec2& center, const Vec2& aim, BodyComponent* pBody, float functionalCapacity);
	/// Called by our parent module
	virtual void postPhysUpdate(const Vec2& center, const Vec2& aim, BodyComponent* pBody);
	/// <summary>
	/// Called before physics update if this weapon should fire this tick.
	/// </summary>
	virtual void preShot(const ShotData& data) = 0;
	/// <summary>
	/// Called after physics update if this weapon fired this tick.
	/// </summary>
	virtual void postShot(const ShotData& data) = 0;
	///Does damage to target. Leave last param blank for no visual effects.
	static void damage(IOManager* pMessageReciever, int ioTargetPos, int damageAmount, int ioCausePos, Team team, const Vec2& collisionPoint, const Vec2& fromDirection, const String& effect = "", bool bleed = false);
	///Gets the decoration object corresponding to this weapon.
	QuadComponent* getDecor();
	///Tell this weapon which team it is working for.
	void setParentData(BodyComponent* parent);

protected:
	///Team that owns this wep.
	Team m_team;
	const FixtureComponent* m_pTempParent;
	int m_shots;//how many shots we do upon each fire command
	String m_effectName;
	int m_shotsInSpread; //how many shots per spread
	float m_fireArc;
	int m_damagePerShot;
	float m_range;
	int m_collisions;//how many collisions should we do? MODULE PENETRATION LOGIC
	//TODO m_collisions is not used in the laser weapon type
	/// Damages the specified fixture (which has a module). Meant to be called by a weapon only. Leave last param blank for no visual effects.
	void damage(b2Fixture* pFixtureTakingDamage, int damageAmount, const Vec2& collisionPoint = Vec2(0, 0), const Vec2& fromDirection = Vec2(0, 0));


	/// <summary>
	/// Used by laser and grapple.
	/// </summary>
	BodyComponent* m_pParentBody;
private:
	Vec2 randArc(const Vec2& center, const Vec2& aim) const;
	QuadComponent m_decor;//the weapon sprite

	leon::Sound m_startSound;//when we start firing
	leon::Sound m_shotSound;//when we take a shot
	leon::Sound m_endSound;//when last shot is taken

	Timer m_shotTimer;// Records how often we can "shot"
	int m_shotsRemain;//how many shots we have remaining on this fire

	float m_energy;// How much do we consume from parent pool.
	float m_ballistic;// How much do we consume from parent pool.
	float m_missiles;//How much do we consume from parent pool.
	Timer m_fireTimer;//Records how often we can fire
	float m_fireDelay;//time in seconds that it takes to fire again
	bool m_shotThisTick;//did we fire this tick?

};

/// Blueprint for Weapon.
struct WeaponData : public BlueprintableData
{
	WeaponData() :
		energyConsumption(1),
		ballisticConsumption(0),
		missileConsumption(0),

		shots(1),
		shotsInSpread(1),
		fireArc(1),//degrees
		damage(50),

		shotDelay(1.f),
		fireDelay(1.f),
		collisions(1),
		range(45.f),
		effectName("LowSparks")
	{
		title = "WEAPON_DEFAULT_TITLE";

		weaponQuad.texName = "weapons/LaserWeapon";
		weaponQuad.layer = GraphicsLayer::ShipAppendagesUpper;
		weaponQuad.dimensions = sf::Vector2f(256, 256);
		weaponQuad.permanentRot = -90.f;
	}
	float energyConsumption;//energy consumed per fire
	float ballisticConsumption;//ballistics consumed per fire
	float missileConsumption;//missiles consumed per fire

	leon::Sound startSound, shotSound, endSound;

	int shots;//how many shots per fire
	int shotsInSpread;// if this weapon is a shotgun, how many shots in its spread
	float fireArc;
	int damage;//damage we do per shot
	int startDamage;

	float shotDelay;//how much time between shots
	float fireDelay;//reload time
	int collisions;//TODO works for projectiles atm, but not lasers
	float range;//how far can this shoot
	String effectName;

	QuadComponentData weaponQuad;

	///Returns null and prints out the file and line that attempted to instantiate this virtual object.
	virtual Weapon* generate() const = 0;
	///Create new copy of this data object and prints out the file and line that attempted to clone this data object.
	virtual WeaponData* clone() const = 0;

	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
	/// <summary>
	/// Upgrades this blueprint;
	/// </summary>
	virtual void applyUpgrade(UpgradeType type);
};

#endif // WEAPON_HPP
