#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "stdafx.hpp"
#include "Pool.hpp"
#include "QuadComponent.hpp"
#include "Globals.hpp"
#include "SoundData.hpp"

struct WeaponData;

/// <summary>
/// Base Class for all weapon types.
/// </summary>
class Weapon
{
public:
	Weapon(const WeaponData& rData);
	virtual ~Weapon();
	/// <summary>
	/// for when the controller wants to fire. This activates and consumes energy and ammo,
	/// and sets the weapon so it will fire on preShot and postShot.
	/// returns true if fired
	/// </summary>
	/// <param name="pEnergy">Parent pool</param>
	/// <param name="pBall">Parent pool</param>
	/// <param name="pMis">Parent pool</param>
	/// <returns></returns>
	bool fire(Pool<Energy>* pEnergy, Pool<Ballistic>* pBall, Pool<Missiles>* pMis);
	/// <summary>
	/// Called by our parent module
	/// </summary>
	/// <param name="pBody">The parent body.</param>
	void prePhysUpdate(const b2Vec2& center, const b2Vec2& aim, float32 radCCW, b2Body* pBody);
	/// <summary>
	/// Called by our parent module
	/// </summary>
	/// <param name="pBody">The parent body.</param>
	void postPhysUpdate(const b2Vec2& center, const b2Vec2& aim, float32 radCCW, b2Body* pBody);
	//// <summary>
	/// Called before physics update if this weapon should fire this tick
	/// Overwrite this when making a new weapon.
	/// </summary>
	/// <param name="center">The center of the Module we are attached to</param>
	/// <param name="aim">Aim of Controller</param>
	/// <param name="radCCW">Angle of Parent Module</param>
	virtual void preShot(const b2Vec2& center, const b2Vec2& aim, float radCCW) = 0;
	//// <summary>
	/// Called after physics update if this weapon fired this tick
	/// Overwrite this when making a new weapon.
	/// </summary>
	/// <param name="center">The center of the Module we are attached to</param>
	/// <param name="aim">Aim of Controller</param>
	/// <param name="radCCW">Angle of Parent Module</param>
	virtual void postShot(const b2Vec2& center, const b2Vec2& aim, float radCCW) = 0;
protected:	
	/// <summary>
	/// Damages the specified fixture (which has a module)
	/// </summary>
	/// <param name="pFix">The p fix.</param>
	/// <param name="damage">The damage.</param>
	void damage(b2Fixture* pFix, int damage);
	b2Body* m_pBody;

	int m_damage;
	float m_range;
private:
	QuadComponent m_decor;//the weapon sprite

	SoundData m_startSound;//when we start firing
	SoundData m_shotSound;//when we take a shot
	SoundData m_endSound;//when last shot is taken
	

	Timer m_shotTimer;// Records how often we can "shot"
	int m_shotsRemain;//how many shots we have remaining on this fire
	int m_shots;//how many shots we do upon each fire command
	
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
		ener(3),
		ball(0),
		mis(0),

		shots(5),
		damage(50),

		shotDelay(0.09f),
		fireDelay(1.5f),

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

	float range;//how far can this shoot

	QuadComponentData weaponQuad;


	virtual Weapon* generate() const
	{
		WeaponData copy(*this);
		std::cout << "\n" << FILELINE;
		return NULL;
	}
	virtual WeaponData* clone() const
	{
		std::cout << "\n" << FILELINE;
		return new WeaponData(*this);
	}
};

#endif // WEAPON_HPP
