#ifndef TURRET_HPP
#define TURRET_HPP

#include "ShipModule.hpp"
#include "Weapon.hpp"

struct TurretData;

class Turret : public ShipModule
{
public:
	Turret(const TurretData& rData);
	virtual ~Turret();

	void setWep(sptr<const WeaponData> spWep);
	void removeWep();

	void prePhysUpdate() final;
	void postPhysUpdate() final;
	virtual void setAim(const b2Vec2& rTarget);
	virtual void directive(std::map<Directive, bool>& rIssues);
	virtual void stealthOn(bool toggle);

protected:
private:
	sptr<Weapon> m_spWep;
	int m_shotsRemain;//shots remaining in this volley

	b2Vec2 m_lastAim; //current aim of controller
	float32 m_lastAngle;//used to preserve the orientation of a broken turret
};


struct TurretData : public ShipModuleData
{
	TurretData() :
		ShipModuleData(),
		startEmpty(false)//should we start with no weapon
	{
		baseDecor.texName = "turret/turret_base.png";
		baseDecor.animSheetName = "turret/turret_base.acfg";
	}

	bool startEmpty;
	sptr<const WeaponData> startWep;

	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		TurretData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new Turret(copy);
	}
	virtual ModuleData* clone() const
	{
		return new TurretData(*this);
	}
};

#endif // TURRET_HPP
