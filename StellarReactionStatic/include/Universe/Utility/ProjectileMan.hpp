#ifndef PROJECTILEMAN_HPP
#define PROJECTILEMAN_HPP

#include "stdafx.hpp"
#include "NonCopyable.hpp"

class Projectile;

/**MANAGES PROJECTILES**/
class ProjectileMan : NonCopyable
{
public:
	ProjectileMan();
	virtual ~ProjectileMan();

	Projectile* getProjectile(const std::string& rBPName);
	void freeProjectile(Projectile* pProj);
	void addNew(const std::string& rBPName);

	void prePhysUpdate();
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
