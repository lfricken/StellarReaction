#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "stdafx.hpp"
#include "Pool.hpp"
#include "QuadComponent.hpp"
#include "Globals.hpp"
#include "SoundData.hpp"

struct WeaponData;


///ADD ANOTHER TIMER THAT SIMULATES RELOADING A MAGAZINE**/


class Weapon
{
public:
	Weapon(const WeaponData& rData);
	virtual ~Weapon();

	bool fire(Pool<Energy>* pEnergy, Pool<Ballistic>* pBall, Pool<Missiles>* pMis);//returns true if we fire

	void prePhysUpdate(const b2Vec2& center, const b2Vec2& aim, float32 radCCW, b2Body* pBody);
	void postPhysUpdate(const b2Vec2& center, const b2Vec2& aim, float32 radCCW, b2Body* pBody);

	virtual void preShot(const b2Vec2& center, const b2Vec2& aim, float radCCW) = 0;
	virtual void postShot(const b2Vec2& center, const b2Vec2& aim, float radCCW) = 0;

protected:
	void damage(b2Fixture* pFix, int damage);
	b2Body* m_pBody;

	int m_damage;
	float m_range;
private:
	QuadComponent m_decor;//the weapon sprite

	SoundData m_startSound;
	SoundData m_shotSound;
	SoundData m_endSound;

	Timer m_shotTimer;
	int m_shotsRemain;
	int m_shots;

	Energy m_energy;
	Ballistic m_ballistic;
	Missiles m_missiles;

	Timer m_fireTimer;
	float m_fireDelay;
	bool m_shotThisTick;
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
	int damage;//damage we do total

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
