#include "ProjectileMan.hpp"
#include "Globals.hpp"
#include "Projectile.hpp"
#include "BlueprintLoader.hpp"
#include "Debugging.hpp"



ProjectileMan::ProjectileMan()
{
	// allow projectile manager to send and recieve events
	IOComponentData data(&getGame()->getUniverse().getUniverseIO());
	data.name = "projectileManager";
	m_ioComponent.reset(new IOComponent(data, &ProjectileMan::input, this));
}
ProjectileMan::~ProjectileMan()
{

}
void ProjectileMan::launchNewProjectile(const Projectile::LaunchData& launchParams)
{
	sf::Packet data;
	data << launchParams.blueprint;
	launchParams.intoPacket(&data);

	Message message(m_ioComponent->getName(), "launch", data, 0.f, false);
	Message::SendUniverse(message);
}
void ProjectileMan::input(String rCommand, sf::Packet data)
{
	if(rCommand == "launch")
	{
		String blueprint;
		Projectile::LaunchData launchData;

		data >> blueprint;
		launchData.fromPacket(&data);

		localLaunch(blueprint, launchData);
	}
}
void ProjectileMan::localLaunch(String blueprint, const Projectile::LaunchData& launchData)
{
	Print << "\nProjectile fired!";
}
Projectile* ProjectileMan::getProjectile(const String& blueprint)
{
	//Find if the blueprint already exists in the list.
	auto it = m_projectileList.find(blueprint);
	if(it == m_projectileList.cend())
	{
		m_projectileList[blueprint] = IndexedList();
		List<sptr<Projectile> >& rProjs = std::get<Projectiles>(m_projectileList[blueprint]);
		int& rFreeIndex = std::get<FreeIndexes>(m_projectileList[blueprint]);
		rFreeIndex = 0;
		it = m_projectileList.find(blueprint);
	}

	IndexedList& rValue = it->second;
	int& rFreeIndex = std::get<FreeIndexes>(rValue);
	List<sptr<Projectile> >& rProjs = std::get<Projectiles>(rValue);
	    
	//if we don't have enough projectiles, add some more
	if(rFreeIndex >= (signed)rProjs.size())
		addNew(blueprint);

	Projectile* pTemp = rProjs[rFreeIndex].get();
	++rFreeIndex;
	return pTemp;
}
void ProjectileMan::freeProjectile(Projectile* projectile)
{
	IndexedList& rMap = m_projectileList[projectile->getTitle()];
	List<sptr<Projectile> >& rList = std::get<Projectiles>(rMap);
	int& rFreeIndex = std::get<FreeIndexes>(rMap);
	int freed = 0;
	for(freed = 0; freed < (signed)rList.size(); ++freed)
		if(rList[freed].get() == projectile)
			break;

	if(freed == rList.size())
		WARNING;

	--rFreeIndex;
	auto temp = rList[rFreeIndex];
	rList[rFreeIndex] = rList[freed];
	rList[freed] = temp;
}
void ProjectileMan::addNew(const String& blueprint)
{
	List<sptr<Projectile> >& rProjs = std::get<Projectiles>(m_projectileList[blueprint]);
	rProjs.push_back(sptr<Projectile>(getGame()->getUniverse().getBlueprints().getProjectileSPtr(blueprint)->generate()));
}
void ProjectileMan::prePhysUpdate()
{
	for(auto bpIt = m_projectileList.begin(); bpIt != m_projectileList.end(); ++bpIt)
	{
		List<sptr<Projectile> >& rProjs = std::get<Projectiles>(bpIt->second);
		for(auto it = rProjs.begin(); it != rProjs.end(); ++it)
			(*it)->prePhysUpdate();
	}
}
void ProjectileMan::postPhysUpdate()
{
	for(auto bpIt = m_projectileList.begin(); bpIt != m_projectileList.end(); ++bpIt)
	{
		List<sptr<Projectile> >& rProjs = std::get<Projectiles>(bpIt->second);
		for(auto it = rProjs.begin(); it != rProjs.end(); ++it)
			(*it)->postPhysUpdate();
	}
}