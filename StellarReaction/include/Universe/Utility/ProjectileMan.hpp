#ifndef PROJECTILEMAN_HPP
#define PROJECTILEMAN_HPP

#include "stdafx.hpp"
class Projectile;

/**MANAGES PROJECTILES**/
class ProjectileMan
{
public:
	ProjectileMan();
	virtual ~ProjectileMan();

	Projectile* getProjectile(const std::string& rBPName);
	void addNew(const std::string& rBPName);

protected:
private:
    typedef std::pair<std::vector<sptr<Projectile> >, int> IndexedList;
	std::map<std::string, IndexedList> m_projectileList;//access[projectileName][index]
};

#endif // PROJECTILEMAN_HPP
