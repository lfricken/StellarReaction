#include "BlueprintLoader.hpp"

#include "Directory.hpp"
#include "Debugging.hpp"

#include "Weapon.hpp"
#include "Module.hpp"
#include "Chunk.hpp"
#include "Projectile.hpp"
#include "Particles.hpp"

BlueprintLoader::BlueprintLoader()
{

}
BlueprintLoader::~BlueprintLoader()
{

}
void BlueprintLoader::loadBlueprints(const String& rDir)
{
	List<Pair<String, String> > weapons = getGame()->getDir().getAllFiles(rDir + "weapons", ".bp");
	List<Pair<String, String> > modules = getGame()->getDir().getAllFiles(rDir + "modules", ".bp");
	List<Pair<String, String> > chunks = getGame()->getDir().getAllFiles(rDir + "chunks", ".bp");
	List<Pair<String, String> > projectiles = getGame()->getDir().getAllFiles(rDir + "projectiles", ".bp");
	List<Pair<String, String> > particles = getGame()->getDir().getAllFiles(rDir + "particles", ".bp");

	for(auto it = weapons.begin(); it != weapons.end(); ++it)
		storeFromFile<WeaponData>(it->first, it->second, m_wepBP);

	for(auto it = modules.begin(); it != modules.end(); ++it)
		storeFromFile<ModuleData>(it->first, it->second, m_modBP);

	for(auto it = chunks.begin(); it != chunks.end(); ++it)
		storeFromFile<ChunkData>(it->first, it->second, m_cnkBP);

	for(auto it = projectiles.begin(); it != projectiles.end(); ++it)
		storeFromFile<ProjectileData>(it->first, it->second, m_prjBP);

	for(auto it = particles.begin(); it != particles.end(); ++it)
		storeFromFile<Particles>(it->first, it->second, m_particlesBP);

	createTeamBlueprintCopies(m_wepBP);
	createTeamBlueprintCopies(m_modBP);
}
sptr<const ChunkData> BlueprintLoader::getChunkSPtr(const String& rBPName) const
{
	return getData<ChunkData>(rBPName, m_cnkBP);
}
sptr<const Particles> BlueprintLoader::getParticleSPtr(const String& rBPName) const
{
	return getData<Particles>(rBPName, m_particlesBP);
}
sptr<const ProjectileData> BlueprintLoader::getProjectileSPtr(const String& rBPName) const
{
	return getData<ProjectileData>(rBPName, m_prjBP);
}
sptr<const ModuleData> BlueprintLoader::getModuleSPtr(const String& rBPName) const
{
	return getData<ModuleData>(rBPName, m_modBP);
}
sptr<const WeaponData> BlueprintLoader::getWeaponSPtr(const String& rBPName) const
{
	return getData<WeaponData>(rBPName, m_wepBP);
}
sptr<const StaticDecorData> BlueprintLoader::getStaticDecorSPtr(const String& rBPName) const
{
	return getData<StaticDecorData>(rBPName, m_sdcBP);
}
sptr<const DynamicDecorData> BlueprintLoader::getDynamicDecorSPtr(const String& rBPName) const
{
	return getData<DynamicDecorData>(rBPName, m_ddcBP);
}
int BlueprintLoader::upgrade(String bpName, UpgradeType type, Team team)
{
	String bluePrintName = bpName;// +String((int)team);
	int upgradeNum = 0;

	for each(auto pair in m_wepBP)
	{
		if(pair.first == bluePrintName)
		{
			pair.second->applyUpgrade(type);
			break;
		}
	}
	for each(auto pair in m_modBP)
	{
		if(pair.first == bluePrintName)
		{
			pair.second->applyUpgrade(type);
			upgradeNum = pair.second->upgradeLevel;
			break;
		}
	}

	return upgradeNum;
}
void BlueprintLoader::addBp(String bpName, sptr<const ModuleData> bp)
{
	m_modBP[bpName] = sptr<ModuleData>(bp->clone());
}
void BlueprintLoader::addBp(String bpName, sptr<const ChunkData> bp)
{
	m_cnkBP[bpName] = sptr<ChunkData>(bp->clone());
}
