#ifndef PROJECTILEMAN_HPP
#define PROJECTILEMAN_HPP

#include "stdafx.hpp"
#include "NonCopyable.hpp"

class Projectile;

/*
* ProjectileMan Class:
* Manages projectiles.
*/
class ProjectileMan : NonCopyable
{
public:
	ProjectileMan();
	virtual ~ProjectileMan();
	///Returns a pointer to a projectile for the given blueprint name.
	Projectile* getProjectile(const std::string& rBPName);
	///Frees the given projectile.
	void freeProjectile(Projectile* pProj);
	///Adds new projectile from blueprint.
	void addNew(const std::string& rBPName);
	///Actions to process on object before performing physics updates.
	void prePhysUpdate();
	///Actions to process on object after performing physics updates.
	void postPhysUpdate();

protected:
private:
	enum
	{
		List = 0,
		FreeIndex = 1,
	};
    typedef std::tuple<std::vector<sptr<Projectile> >, int> IndexedList;
	std::map<std::string, IndexedList> m_projectileList;//access[projectileName][index]
};

#endif // PROJECTILEMAN_HPP
