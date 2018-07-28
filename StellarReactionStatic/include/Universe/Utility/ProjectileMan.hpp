#ifndef PROJECTILEMAN_HPP
#define PROJECTILEMAN_HPP

#include "stdafx.hpp"
#include "NonCopyable.hpp"

class Projectile;

/// Stores projectiles and creates new ones when needed.
class ProjectileMan : NonCopyable
{
public:
	ProjectileMan();
	virtual ~ProjectileMan();
	///Returns a pointer to a projectile for the given blueprint name.
	///Frees the given projectile.
	void freeProjectile(Projectile* pProj);
	///Adds new projectile from blueprint.
	void addNew(const String& rBPName);
	///Actions to process on object before performing physics updates.
	void prePhysUpdate();
	///Actions to process on object after performing physics updates.
	void postPhysUpdate();

	void launch(const String& rBPName);

	IOComponent m_ioComponent;

protected:
	Projectile* getProjectile(const String& rBPName);

private:
	enum
	{
		Vec = 0,
		FreeIndex = 1,
	};
    typedef std::tuple<List<sptr<Projectile> >, int> IndexedList;
	std::map<String, IndexedList> m_projectileList;//access[projectileName][index]
};

#endif // PROJECTILEMAN_HPP
