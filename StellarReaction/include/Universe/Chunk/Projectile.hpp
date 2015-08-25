#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "BodyComponent.hpp"
#include "ProjectileModule.hpp"
#include "QuadComponent.hpp"
#include "Pool.hpp"

struct ProjectileData;

class Projectile
{
public:
	Projectile(const ProjectileData& rData);
	virtual ~Projectile();

	void launch(const b2Vec2& rStart, const b2Vec2& rVel, float radCCW, float radCCWps, const Message& rPayload);// coord, linVel, rot, rotVel
	void reset();//this projectile will go back into projectile pool

	virtual void prePhysUpdate();
	virtual void postPhysUpdate();

	const std::string& getTitle() const;

protected:
	BodyComponent m_body;
	std::vector<sptr<ProjectileModule> > m_modules;
	std::string m_title;
	Timer m_timer;
	bool m_inPlay;
private:
	Pool<Ballistic> m_ballisticPool;
	Pool<Zoom> m_zoomPool;
	Pool<Missiles> m_missilePool;
	Pool<Energy> m_energyPool;
};

struct ProjectileData
{
	ProjectileData() :
		body(),
		moduleData()
	{
		title = "NO_TITLE";
		body.angularDampening = 0;
		body.coords = b2Vec2(0, 5);
		body.isBullet = true;
		body.isDynamic = true;
		body.linearDampening = 0;
		body.rotation = 0;
		body.startAwake = true;
		lifetime = 0.5f;
	}
	std::string title;
	PoolData<Missiles> missileData;
	PoolData<Ballistic> ballisticData;
	PoolData<Energy> energyData;
	PoolData<Zoom> zoomData;

	BodyComponentData body;
	std::vector<sptr<const ModuleData> > moduleData;
	float lifetime;//time in seconds that the projectile poof out of existence if not already colided

	virtual Projectile* generate() const
	{
		ProjectileData copy(*this);
		return new Projectile(copy);
	}
	virtual ProjectileData* clone() const
	{
		return new ProjectileData(*this);
	}
};

#endif // PROJECTILE_HPP
