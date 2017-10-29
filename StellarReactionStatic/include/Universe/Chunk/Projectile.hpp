#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "BodyComponent.hpp"
#include "ProjectileModule.hpp"
#include "QuadComponent.hpp"
#include "RangeList.hpp"
#include "NonCopyable.hpp"


struct ProjectileData;


/// A Chunk that travels to do damage to other objects.
class Projectile : public ModuleParent, NonCopyable
{
public:
	Projectile(const ProjectileData& rData);
	virtual ~Projectile();
	///Launches projectile with given coordinates, direction, damage and lifetime.
	void launch(const Vec2& rStart, const Vec2& rVel, float radCCW, float radCCWps, float lifetime, int damage, const FixtureComponent* pParent, int collisions);
	///Return this projectile back into projectile pool of parent.
	void reset();
	///Actions to process on object before performing physics updates.
	virtual void prePhysUpdate();
	///Actions to process on object after performing physics updates.
	virtual void postPhysUpdate();
	///Return the title of this object.
	const String& getTitle() const;


	virtual RangeList* getRanges();
protected:
	BodyComponent m_body;
	List<sptr<ProjectileModule> > m_modules;
	String m_title;
	Timer m_timer;
	bool m_inPlay;
private:
	RangeList ranges;
};

///Initiailze Projectile.
struct ProjectileData : public ModuleParentData
{
	ProjectileData() :
		body(),
		moduleData()
	{
		title = "PROJECTILE_DEFAULT_TITLE";
		body.angularDampening = 0;
		body.coords = Vec2(0, 5);
		body.isBullet = true;
		body.isDynamic = true;
		body.syncedNetwork = false;
		body.linearDampening = 0;
		body.rotation = 0;
		body.startAwake = true;
		lifetime = 0.5f;
	}

	RangeListData rangesData;

	BodyComponentData body;
	List<sptr<const ModuleData> > moduleData;
	float lifetime;//time in seconds that the projectile poof out of existence if not already colided

	///Create Projectile object from this data object.
	virtual Projectile* generate() const
	{
		ProjectileData copy(*this);
		return new Projectile(copy);
	}
	///Create new copy of this data object.
	virtual ProjectileData* clone() const
	{
		return new ProjectileData(*this);
	}
	///Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root);

	MyType(ProjectileData, ProjectileData);
};

#endif // PROJECTILE_HPP
