#ifndef PROJECTILEMAN_HPP
#define PROJECTILEMAN_HPP

#include "stdafx.hpp"
#include "NonCopyable.hpp"
#include "Projectile.hpp"

/// Stores projectiles and creates new ones when needed.
class ProjectileMan : NonCopyable
{
public:
	ProjectileMan();
	virtual ~ProjectileMan();
	
	/// <summary>
	/// Returns the given projectile to the projectile cache.
	/// </summary>
	void freeProjectile(Projectile* projectile);

	/// <summary>
	/// Sends a global message to launch a projectile.
	/// </summary>
	void launchNewProjectile(const Projectile::LaunchData& launchData);

	void prePhysUpdate();
	void postPhysUpdate();
protected:

	virtual void input(String command, sf::Packet data);

	/// <summary>
	/// Launch the projectile into the universe on the local machine.
	/// </summary>
	void localLaunch(String blueprint, const Projectile::LaunchData& launchData);

	/// <summary>
	/// Returnt the requested projectile type.
	/// </summary>
	Projectile* getProjectile(const String& blueprint);

	/// <summary>
	/// Add a new instance of this kind of projectile.
	/// </summary>
	void addNew(const String& blueprint);
private:

	sptr<IOComponent> m_ioComponent;

	enum
	{
		Projectiles = 0,
		FreeIndexes = 1,
	};
    typedef std::tuple<List<sptr<Projectile> >, int> IndexedList;
	std::map<String, IndexedList> m_projectileList;//access[projectileName][index]
};

#endif // PROJECTILEMAN_HPP
