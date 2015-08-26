#ifndef PROJECTILEMODULE_HPP
#define PROJECTILEMODULE_HPP

#include "Sensor.hpp"
#include "QuadComponent.hpp"

struct ProjectileModuleData;

class ProjectileModule : public Sensor
{
public:
	ProjectileModule(const ProjectileModuleData& rData);
	virtual ~ProjectileModule();

	void postPhysUpdate();
	void setPayload(int damage, const FixtureComponent* pParent, int collisions);

	virtual void entered(FixtureComponent* pOther);
	virtual void exited(FixtureComponent* pOther);
	virtual bool shouldTerminate() const;

protected:
	bool m_freeThisProjectile;
	int m_damage;
	const b2Body* m_pParent;
	std::vector<sptr<GraphicsComponent> > m_decors;
	int m_currentCollisions;//how many collisions have we had so far
	int m_maxCollisions;//how many collisions should we do

	/// <summary>
	/// Damages the specified fixture (which has a module)
	/// </summary>
	void damage(FixtureComponent* pFix, int damage);
private:

};


struct ProjectileModuleData : public SensorData
{
	ProjectileModuleData() :
		SensorData(),
		baseDecor()
	{
		fixComp.isSensor = true;
		fixComp.shape = leon::Shape::Rectangle;
		fixComp.colCategory = Category::Projectile;
		fixComp.colMask = Mask::Projectile;
		baseDecor.layer = GraphicsLayer::Projectiles;
	}

	QuadComponentData baseDecor;

	virtual Module* generate(b2Body* pBody, PoolCollection stuff) const
	{
		ProjectileModuleData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		return new ProjectileModule(copy);
	}
	virtual ModuleData* clone() const
	{
		return new ProjectileModuleData(*this);
	}
};

#endif // PROJECTILEMODULE_HPP
