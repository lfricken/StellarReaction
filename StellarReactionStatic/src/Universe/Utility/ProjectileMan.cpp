#include "ProjectileMan.hpp"
#include "Globals.hpp"
#include "Projectile.hpp"
#include "BlueprintLoader.hpp"
#include "Debugging.hpp"



ProjectileMan::ProjectileMan() : m_ioComponent(
{

}
ProjectileMan::~ProjectileMan()
{

}
Projectile* ProjectileMan::getProjectile(const String& rBPName)
{
	//Find if the blueprint already exists in the list.
	auto it = m_projectileList.find(rBPName);
	if(it == m_projectileList.cend())
	{
		m_projectileList[rBPName] = IndexedList();
		List<sptr<Projectile> >& rProjs = std::get<Vec>(m_projectileList[rBPName]);
		int& rFreeIndex = std::get<FreeIndex>(m_projectileList[rBPName]);
		rFreeIndex = 0;
		it = m_projectileList.find(rBPName);
	}

	IndexedList& rValue = it->second;
	int& rFreeIndex = std::get<FreeIndex>(rValue);
	List<sptr<Projectile> >& rProjs = std::get<Vec>(rValue);
	    
	//if we don't have enough projectiles, add some more
	if(rFreeIndex >= (signed)rProjs.size())
		addNew(rBPName);

	Projectile* pTemp = rProjs[rFreeIndex].get();
	++rFreeIndex;
	return pTemp;
}
void ProjectileMan::freeProjectile(Projectile* pProj)
{
	IndexedList& rMap = m_projectileList[pProj->getTitle()];
	List<sptr<Projectile> >& rList = std::get<Vec>(rMap);
	int& rFreeIndex = std::get<FreeIndex>(rMap);
	int freed = 0;
	for(freed = 0; freed < (signed)rList.size(); ++freed)
		if(rList[freed].get() == pProj)
			break;

	if(freed == rList.size())
		WARNING;

	--rFreeIndex;
	auto temp = rList[rFreeIndex];
	rList[rFreeIndex] = rList[freed];
	rList[freed] = temp;
}
void ProjectileMan::addNew(const String& rBPName)
{
	List<sptr<Projectile> >& rProjs = std::get<Vec>(m_projectileList[rBPName]);
	rProjs.push_back(sptr<Projectile>(game.getUniverse().getBlueprints().getProjectileSPtr(rBPName)->generate()));
}
void ProjectileMan::prePhysUpdate()
{
	for(auto bpIt = m_projectileList.begin(); bpIt != m_projectileList.end(); ++bpIt)
	{
		List<sptr<Projectile> >& rProjs = std::get<Vec>(bpIt->second);
		for(auto it = rProjs.begin(); it != rProjs.end(); ++it)
			(*it)->prePhysUpdate();
	}
}
void ProjectileMan::postPhysUpdate()
{
	for(auto bpIt = m_projectileList.begin(); bpIt != m_projectileList.end(); ++bpIt)
	{
		List<sptr<Projectile> >& rProjs = std::get<Vec>(bpIt->second);
		for(auto it = rProjs.begin(); it != rProjs.end(); ++it)
			(*it)->postPhysUpdate();
	}
}