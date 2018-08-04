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
	void setWep(String wepName);
	///Remove the weapon from this turret.
	void removeWep();
	///Actions to process on object before performing physics updates.
	virtual void prePhysUpdate();
	///Actions to process on object after performing physics updates.
	virtual void postPhysUpdate();
	///Set the aim of the turret.
	virtual void setAim(const Vec2& rTarget);
	///Send commands to a target.
	virtual void directive(const CommandInfo& commands);
	///Set the stealth state.
	virtual void toggleStealth(bool toggle);
	/// <summary>
	/// The weapon on this turret.
	/// </summary>
	const Weapon* getWeapon();

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

	/// <summary>
	/// Sets this blueprint to be on a specific team.
	/// </summary>
	virtual void setTeam(Team team);

	int controlGroup;
	String startWep;
	///Create Turret object from this data object.
	virtual Module* generate(GenerateParams params) const
	{
		return generateSub<Turret, TurretData>(params, this);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new TurretData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);
};


#endif // TURRET_HPP
