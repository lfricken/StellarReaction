#include "ProjectileMan.hpp"
#include "Globals.hpp"
#include "Projectile.hpp"
#include "BlueprintLoader.hpp"

using namespace std;

ProjectileMan::ProjectileMan()
{

}
ProjectileMan::~ProjectileMan()
{

}
Projectile* ProjectileMan::getProjectile(const std::string& rBPName)
{
	//Find if the blueprint already exists in the list.
	auto it = m_projectileList.find(rBPName);
	if(it == m_projectileList.cend())
	{
		m_projectileList[rBPName] = IndexedList();
		vector<sptr<Projectile> >& rProjs = get<List>(m_projectileList[rBPName]);
		int& rFreeIndex = get<FreeIndex>(m_projectileList[rBPName]);
		rFreeIndex = 0;
		it = m_projectileList.find(rBPName);
		cout << "\nnew type!";
	}

	IndexedList& rValue = it->second;
	int& rFreeIndex = get<FreeIndex>(rValue);
	vector<sptr<Projectile> >& rProjs = get<List>(rValue);
	    
	//if we don't have enough projectiles, add some more
	if(rFreeIndex >= rProjs.size())
		addNew(rBPName);

	Projectile* pTemp = rProjs[rFreeIndex].get();
	++rFreeIndex;
	return pTemp;
}
void ProjectileMan::freeProjectile(Projectile* pProj)
{
	std::cout << "\nFreed";
	IndexedList& rMap = m_projectileList[pProj->getTitle()];
	std::vector<sptr<Projectile> >& rList = get<List>(rMap);
	int& rFreeIndex = get<FreeIndex>(rMap);
	int freed = 0;
	for(freed = 0; freed < (signed)rList.size(); ++freed)
		if(rList[freed].get() == pProj)
			break;

	if(freed == rList.size())
		cout << FILELINE;

	--rFreeIndex;
	auto temp = rList[rFreeIndex];
	rList[rFreeIndex] = rList[freed];
	rList[freed] = temp;
}
void ProjectileMan::addNew(const std::string& rBPName)
{
	cout << "\nAdded New";
	vector<sptr<Projectile> >& rProjs = get<List>(m_projectileList[rBPName]);
	rProjs.push_back(sptr<Projectile>(game.getUniverse().getBlueprints().getProjectileSPtr(rBPName)->generate()));
}
void ProjectileMan::prePhysUpdate()
{
	for(auto bpIt = m_projectileList.begin(); bpIt != m_projectileList.end(); ++bpIt)
	{
		std::vector<sptr<Projectile> >& rProjs = get<List>(bpIt->second);
		for(auto it = rProjs.begin(); it != rProjs.end(); ++it)
			(*it)->prePhysUpdate();
	}
}
void ProjectileMan::postPhysUpdate()
{
	for(auto bpIt = m_projectileList.begin(); bpIt != m_projectileList.end(); ++bpIt)
	{
		std::vector<sptr<Projectile> >& rProjs = get<List>(bpIt->second);
		for(auto it = rProjs.begin(); it != rProjs.end(); ++it)
			(*it)->postPhysUpdate();
	}
}