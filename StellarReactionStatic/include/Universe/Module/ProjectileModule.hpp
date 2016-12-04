#ifndef PROJECTILEMODULE_HPP
#define PROJECTILEMODULE_HPP

#include "Sensor.hpp"
#include "QuadComponent.hpp"

struct ProjectileModuleData;
enum class Team;


/// Detects when colliding with something a potentially dealing damage.
class ProjectileModule : public Sensor
{
public:
	ProjectileModule(const ProjectileModuleData& rData);
	virtual ~ProjectileModule();
	///Actions to process on object after performing physics updates.
	void postPhysUpdate();
	///Set the damage, parent, and number of allowed collisions.
	void setPayload(int damage, const FixtureComponent* pParent, int collisions);
	///Actions to process when fixture component enters our sensor. May be called multiple times in a single step.
	virtual void entered(FixtureComponent* pOther);
	///Actions to process when fixture component exits our sensor. May be called multiple times in a single step.
	virtual void exited(FixtureComponent* pOther);
	///Should this projectile be freed?
	virtual bool shouldTerminate() const;
	///Input commands to this object.
	virtual void input(String rCommand, sf::Packet rData);

protected:
	bool m_freeThisProjectile;
	int m_damage;

	///Which team is this damage coming from?
	Team m_team;
	///Which physics body?
	const b2Body* m_pParent;
	///IO position?
	int m_sourceIOPos;



	List<sptr<GraphicsComponent> > m_decors;
	///how many collisions have we had so far.
	int m_currentCollisions;
	///how many collisions should we do.
	int m_maxCollisions;

private:

};

/// Initialize ProjectileModule
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
	///Create ProjectileModule object from this data object.
	virtual Module* generate(b2Body* pBody, PoolCollection stuff, Chunk* parent) const
	{
		ProjectileModuleData copy(*this);
		copy.pools = stuff;
		copy.fixComp.pBody = pBody;
		copy.chunkParent = parent;
		return new ProjectileModule(copy);
	}
	///Create new copy of this data object.
	virtual ModuleData* clone() const
	{
		return new ProjectileModuleData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	MyType(ModuleData, ProjectileModuleData);
};

#endif // PROJECTILEMODULE_HPP
