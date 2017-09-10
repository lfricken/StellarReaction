#include "BlueprintLoader.hpp"

#include "Directory.hpp"

#include "ModuleRegistration.hpp"
#include "WeaponRegistration.hpp"
#include "ChunkRegistration.hpp"
#include "ProjectileRegistration.hpp"
#include "Debugging.hpp"



BlueprintLoader::BlueprintLoader()
{

}
BlueprintLoader::~BlueprintLoader()
{

}
void BlueprintLoader::loadBlueprints(const String& rDir)
{
	List<Pair<String, String> > weapons = game.getDir().getAllFiles(rDir + "weapons", ".bp");
	List<Pair<String, String> > modules = game.getDir().getAllFiles(rDir + "modules", ".bp");
	List<Pair<String, String> > chunks = game.getDir().getAllFiles(rDir + "chunks", ".bp");
	List<Pair<String, String> > projectiles = game.getDir().getAllFiles(rDir + "projectiles", ".bp");
	List<Pair<String, String> > particles = game.getDir().getAllFiles(rDir + "particles", ".bp");


	for(auto it = weapons.begin(); it != weapons.end(); ++it)
		storeData<WeaponData>(it->first, it->second, m_wepBP);

	for(auto it = modules.begin(); it != modules.end(); ++it)
	{
		storeData<ModuleData>(it->first, it->second, m_modBP);
	}

	for(auto it = chunks.begin(); it != chunks.end(); ++it)
	{
		storeData<ChunkData>(it->first, it->second, m_cnkBP);
	}

	for(auto it = projectiles.begin(); it != projectiles.end(); ++it)
		storeData<ProjectileData>(it->first, it->second, m_prjBP);

	for(auto it = particles.begin(); it != particles.end(); ++it)
		storeData<Particles>(it->first, it->second, m_particlesBP);
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
