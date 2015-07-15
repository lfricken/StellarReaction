#include "ProjectileMan.hpp"
#include "Globals.hpp"
#include "Projectile.hpp"

ProjectileMan::ProjectileMan()
{

}
ProjectileMan::~ProjectileMan()
{

}
Projectile* ProjectileMan::getProjectile(const std::string& rBPName)
{
	/**FIND LIST**/
	auto it = m_projectileList.find(rBPName);
	if(it != m_projectileList.cend())
	{
	    int freeProjectileIndex = it->second.second;
		auto& rProjectiles = it->second.first;
	    ++(it->second.second);
	    /**FIND PROJECTILE IN LIST**/
        //if(freeProjectileIndex == rProjectiles.size())
            //addNew(rBPName);
        return rProjectiles[freeProjectileIndex].get();
	}
	else
    {
        ///ERROR LOG
        std::cout << FILELINE;
		return NULL;
    }
}

