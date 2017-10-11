#ifndef TURRET_HPP
#define TURRET_HPP

#include "ShipModule.hpp"
#include "Weapon.hpp"

struct TurretData;


/// Holds a Weapon that it controls.
class Turret : public ShipModule
{
public:
	Turret(const TurretData& rData);
	virtual ~Turret();
	///Set the weapon for this turret.
	void setWep(sptr<const WeaponData> spWep);
	///Remove the weapon from this turret.
	void removeWep();
	///Actions to process on object before performing physics updates.
	void prePhysUpdate() final;
	///Actions to process on object after performing physics updates.
	void postPhysUpdate() final;
	///Set the aim of the turret.
	virtual void setAim(const Vec2& rTarget);
	///Send commands to a target.
	virtual void directive(const CommandInfo& commands);
	///Set the stealth state.
	virtual void toggleStealth(bool toggle);

protected:
private:
	int m_controlGroup;

	sptr<Weapon> m_spWep;
	int m_shotsRemain;//shots remaining in this volley

	Vec2 m_lastAim; //current aim of controller
	float32 m_lastAngle;//used to preserve the orientation of a broken turret
};

/// Blueprint for Weapon.
struct TurretData : public ShipModuleData
{
	TurretData() :
		ShipModuleData(),
		controlGroup(1)
	{
		baseDecor.texName = "turret/turret_base";
	}

	int controlGroup;
	sptr<const WeaponData> startWep;
	///Create Turret object from this data object.
	virtual Module* generate(b2Body* pBody, RangeList* ranges, Chunk* parent) const
	{
		TurretData copy(*this);
		copy.ranges = ranges;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new Turret(copy);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new TurretData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, TurretData);
};


#endif // TURRET_HPP
