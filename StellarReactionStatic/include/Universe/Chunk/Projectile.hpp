#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "BodyComponent.hpp"
#include "ProjectileModule.hpp"
#include "QuadComponent.hpp"
#include "RangeList.hpp"
#include "NonCopyable.hpp"
#include "ModuleParent.hpp"
#include "ISerializable.hpp"


struct ProjectileData;


/// A Chunk that travels to do damage to other objects.
class Projectile : public ModuleParent, NonCopyable
{
public:
	struct LaunchData : ISerializable
	{
		String blueprint;
		int targetChunkPosition;
		int launcherModuleIoPosition;

		Vec2 startPosition;

		float rotation;
		float rotationRate;
		Vec2 velocity;
		float maxVelocity;
		float acceleration;

		Team team;
		float lifetime;
		int damage;
		int collisions;

		virtual void fromPacket(sf::Packet* data);
		virtual void intoPacket(sf::Packet* data) const;
		virtual sptr<sf::Packet> intoPacket() const;
	};

	Projectile(const ProjectileData& rData);
	virtual ~Projectile();
	
	/// <summary>
	/// Return this projectile back into projectile pool of parent.
	/// </summary>
	void reset();

	virtual void prePhysUpdate();
	virtual void postPhysUpdate();
protected:

	/// <summary>
	/// Launch this projectile.
	/// </summary>
	virtual void launch(const LaunchData& data);

	/// <summary>
	/// Sets payload on each projectile module.
	/// </summary>
	void setPayloadOnModules(int damage, Team team, int launcherModuleIoPosition, int collisions);

	Timer m_timer;//Time till we explode due to no fuel.
	bool m_inPlay;

};

///Initiailze Projectile.
struct ProjectileData : public ModuleParentData
{
	ProjectileData() :
		lifetime(1)
	{

		title = "PROJECTILE_DEFAULT_TITLE";

		bodyComp.angularDampening = 0;
		bodyComp.coords = Vec2(0, 5);
		bodyComp.isBullet = true;
		bodyComp.isDynamic = true;
		bodyComp.syncedNetwork = false;
		bodyComp.linearDampening = 0;
		bodyComp.rotation = 0;
		bodyComp.startAwake = true;
	}

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
};

#endif // PROJECTILE_HPP
