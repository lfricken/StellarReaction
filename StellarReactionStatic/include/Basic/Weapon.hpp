#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "stdafx.hpp"
#include "Pool.hpp"
#include "QuadComponent.hpp"
#include "Globals.hpp"
#include "SoundData.hpp"
#include "ClassRegister.hpp"

struct WeaponData;
class FixtureComponent;

/// <summary>
/// Base Class for all weapon types.
/// Derived weapons override only preshot and postshot
/// </summary>
class Weapon
{
public:
	Weapon(const WeaponData& rData);
	virtual ~Weapon();
	/// <summary>
	/// called when the controller wants to fire. This activates and consumes any ammo,
	/// and sets the weapon so it will fire on preShot and postShot.
	/// returns true if we will fire
	/// </summary>
	bool fire(const FixtureComponent& pParent, Pool<Energy>* pEnergy, Pool<Ballistic>* pBall, Pool<Missiles>* pMis);
	/// <summary>
	/// Called by our parent module
	/// </summary>
	/// <param name="pBody">The parent body.</param>
	void prePhysUpdate(const b2Vec2& center, const b2Vec2& aim, float32 radCCW, b2Body* pBody, float module_orientation);
	/// <summary>
	/// Called by our parent module
	/// </summary>
	/// <param name="pBody">The parent body.</param>
	void postPhysUpdate(const b2Vec2& center, const b2Vec2& aim, float32 radCCW, b2Body* pBody, float module_orientation);
	//// <summary>
	/// Called before physics update if this weapon should fire this tick
	/// Look at laser and projectile weapon.
	/// Overwrite this when making a new weapon.
	/// </summary>
	virtual void preShot(const b2Vec2& center, const b2Vec2& aim, float radCCW, float module_orientation) = 0;
	//// <summary>
	/// Called after physics update if this weapon fired this tick
	/// Overwrite this when making a new weapon.
	/// Look at laser and projectile weapon.
	/// </summary>

	
	virtual void postShot(const b2Vec2& center, const b2Vec2& aim, float radCCW, float module_orientation) = 0;
	QuadComponent* getDecor();

protected:
	b2Body* m_pBody;
	const FixtureComponent* m_pTempParent;
	int m_shots;//how many shots we do upon each fire command
	int m_damage;
	float m_range;
	int m_collisions;//how many collisions should we do? MODULE PENETRATION LOGIC
	//TODO m_collisions is not used in the laser weapon type

	/// <summary>
	/// Damages the specified fixture (which has a module)
	/// </summary>
	void damage(b2Fixture* pFix, int damage);
private:
	QuadComponent m_decor;//the weapon sprite

	SoundData m_startSound;//when we start firing
	SoundData m_shotSound;//when we take a shot
	SoundData m_endSound;//when last shot is taken
	

	Timer m_shotTimer;// Records how often we can "shot"
	int m_shotsRemain;//how many shots we have remaining on this fire
	
	Energy m_energy;// How much do we consume from parent pool.
	Ballistic m_ballistic;// How much do we consume from parent pool.
	Missiles m_missiles;//How much do we consume from parent pool.
	Timer m_fireTimer;//Records how often we can fire
	float m_fireDelay;//time in seconds that it takes to fire again
	bool m_shotThisTick;//did we fire this tick?
};


struct WeaponData
{
	WeaponData() :
		ener(0),
		ball(0),
		mis(0),

		shots(5),
		damage(50),

		shotDelay(0.09f),
		fireDelay(1.5f),
		collisions(3),
		range(45.f)
	{
		weaponQuad.animSheetName = "weapons/laser1.acfg";
		weaponQuad.texName = "weapons/laser1.png";
		weaponQuad.layer = GraphicsLayer::ShipAppendagesUpper;
		weaponQuad.dimensions = sf::Vector2f(128, 260);
		weaponQuad.permanentRot = -90.f;
	}
	Energy ener;//energy consumed per fire
	Ballistic ball;//ballistics consumed per fire
	Missiles mis;//missiles consumed per fire

	SoundData startSound, shotSound, endSound;

	int shots;//how many shots per fire
	int damage;//damage we do per shot

	float shotDelay;//how much time between shots
	float fireDelay;//reload time
	int collisions;//TODO works for projectiles atm, but not lasers
	float range;//how far can this shoot

	QuadComponentData weaponQuad;


	virtual Weapon* generate() const
	{
		WeaponData copy(*this);
		std::cout << FILELINE;
		return NULL;
	}
	virtual WeaponData* clone() const
	{
		std::cout << FILELINE;
		return new WeaponData(*this);
	}

	virtual void loadJson(const Json::Value& root);

	MyType(WeaponData, WeaponData);
};

#endif // WEAPON_HPP
