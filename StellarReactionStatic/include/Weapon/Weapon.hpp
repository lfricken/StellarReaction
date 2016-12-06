#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "stdafx.hpp"
#include "Pool.hpp"
#include "QuadComponent.hpp"
#include "Globals.hpp"
#include "ClassRegister.hpp"
#include "BlueprintData.hpp"
#include "NonCopyable.hpp"
#include "Sound.hpp"

enum class Team;
struct WeaponData;
class FixtureComponent;

struct HitEffect
{
	String effect;
	Vec2 pos;
	Vec2 normal;
};

/// Basic object that provides easier interface for making complex types of weapons.
class Weapon : NonCopyable
{
public:
	Weapon(const WeaponData& rData);
	virtual ~Weapon();
	/// Called when the controller wants to fire. This activates and consumes any ammo,
	/// and sets the weapon so it will fire on preShot and postShot.
	/// returns true if we will fire.
	bool fire(const FixtureComponent& pParent, Pool<Energy>* pEnergy, Pool<Ballistic>* pBall, Pool<Missiles>* pMis);
	/// Called by our parent module
	virtual void prePhysUpdate(const Vec2& center, const Vec2& aim, float32 radCCW, b2Body* pBody, float module_orientation);
	/// Called by our parent module
	virtual void postPhysUpdate(const Vec2& center, const Vec2& aim, float32 radCCW, b2Body* pBody, float module_orientation);
	/// Called before physics update if this weapon should fire this tick
	/// Look at laser and projectile weapon.
	/// Overwrite this when making a new weapon.
	virtual void preShot(const Vec2& center, const Vec2& aim, float radCCW, float module_orientation) = 0;
	/// Called after physics update if this weapon fired this tick
	/// Overwrite this when making a new weapon.
	/// Look at laser and projectile weapon.
	virtual void postShot(const Vec2& center, const Vec2& aim, float radCCW, float module_orientation) = 0;
	///Does damage to target. Leave last param blank for no visual effects.
	static void damage(IOManager* pMessageReciever, int ioTargetPos, int damageAmount, int ioCausePos, Team team, const Vec2& point = Vec2(0,0));
	///Gets the decoration object corresponding to this weapon.
	QuadComponent* getDecor();
	///Tell this weapon which team it is working for.
	void setTeam(Team newTeam);

protected:
	///Team that owns this wep.
	Team m_team;
	b2Body* m_pBody;
	const FixtureComponent* m_pTempParent;
	int m_shots;//how many shots we do upon each fire command
	int m_shotsInSpread; //how many shots per spread
	float m_fireArc;
	int m_damage;
	float m_range;
	int m_collisions;//how many collisions should we do? MODULE PENETRATION LOGIC
	//TODO m_collisions is not used in the laser weapon type
	/// Damages the specified fixture (which has a module). Meant to be called by a weapon only. Leave last param blank for no visual effects.
	void damage(b2Fixture* pFix, int damage, const Vec2& point = Vec2(0,0));
private:
	Vec2 randArc(const Vec2& center, const Vec2& aim) const;
	QuadComponent m_decor;//the weapon sprite

	leon::Sound m_startSound;//when we start firing
	leon::Sound m_shotSound;//when we take a shot
	leon::Sound m_endSound;//when last shot is taken
	

	Timer m_shotTimer;// Records how often we can "shot"
	int m_shotsRemain;//how many shots we have remaining on this fire
	
	Energy m_energy;// How much do we consume from parent pool.
	Ballistic m_ballistic;// How much do we consume from parent pool.
	Missiles m_missiles;//How much do we consume from parent pool.
	Timer m_fireTimer;//Records how often we can fire
	float m_fireDelay;//time in seconds that it takes to fire again
	bool m_shotThisTick;//did we fire this tick?
};

/// Blueprint for Weapon.
struct WeaponData : public BlueprintData
{
	WeaponData() :
		energyConsumption(1),
		ballisticConsumption(0),
		missileConsumption(0),

		shots(1),
		shotsInSpread(1),
		fireArc(0),//degrees
		damage(50),

		shotDelay(1.f),
		fireDelay(1.f),
		collisions(1),
		range(45.f)
	{
		title = "WEAPON_DEFAULT_TITLE";

		weaponQuad.animSheetName = "weapons/LaserWeapon.acfg";
		weaponQuad.texName = "weapons/LaserWeapon.png";
		weaponQuad.layer = GraphicsLayer::ShipAppendagesUpper;
		weaponQuad.dimensions = sf::Vector2f(256, 256);
		weaponQuad.permanentRot = -90.f;
	}
	Energy energyConsumption;//energy consumed per fire
	Ballistic ballisticConsumption;//ballistics consumed per fire
	Missiles missileConsumption;//missiles consumed per fire

	leon::Sound startSound, shotSound, endSound;

	int shots;//how many shots per fire
	int shotsInSpread;// if this weapon is a shotgun, how many shots in its spread
	float fireArc;
	int damage;//damage we do per shot

	float shotDelay;//how much time between shots
	float fireDelay;//reload time
	int collisions;//TODO works for projectiles atm, but not lasers
	float range;//how far can this shoot

	QuadComponentData weaponQuad;

	///Returns null and prints out the file and line that attempted to instantiate this virtual object.
	virtual Weapon* generate() const
	{
		WeaponData copy(*this);
		std::cout << FILELINE;
		return NULL;
	}
	///Create new copy of this data object and prints out the file and line that attempted to clone this data object.
	virtual WeaponData* clone() const
	{
		std::cout << FILELINE;
		return new WeaponData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	MyType(WeaponData, WeaponData);
};

#endif // WEAPON_HPP
