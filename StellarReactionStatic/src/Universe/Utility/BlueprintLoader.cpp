#include "BlueprintLoader.hpp"

#include "Directory.hpp"

#include "ModuleRegistration.hpp"
#include "WeaponRegistration.hpp"
#include "ChunkRegistration.hpp"
#include "ProjectileRegistration.hpp"

using namespace std;

BlueprintLoader::BlueprintLoader()
{

}
BlueprintLoader::~BlueprintLoader()
{

}
void BlueprintLoader::loadBlueprints(const std::string& rDir)
{
	vector<string> weapons = game.getDir().getAllFiles(rDir + "weapons", ".bp");
	vector<string> modules = game.getDir().getAllFiles(rDir + "modules", ".bp");
	vector<string> chunks = game.getDir().getAllFiles(rDir + "chunks", ".bp");
	vector<string> projectiles = game.getDir().getAllFiles(rDir + "projectiles", ".bp");

	for(auto it = weapons.begin(); it != weapons.end(); ++it)
		storeData<WeaponData>(*it, m_wepBP);

	for(auto it = modules.begin(); it != modules.end(); ++it)
		storeData<ModuleData>(*it, m_modBP);

	for(auto it = chunks.begin(); it != chunks.end(); ++it)
		storeData<ChunkData>(*it, m_cnkBP);

	for(auto it = projectiles.begin(); it != projectiles.end(); ++it)
		storeData<ProjectileData>(*it, m_prjBP);
}
sptr<const ChunkData> BlueprintLoader::getChunkSPtr(const std::string& rBPName) const
{
	return getData<ChunkData>(rBPName, m_cnkBP);
}
sptr<const ProjectileData> BlueprintLoader::getProjectileSPtr(const std::string& rBPName) const
{
	return getData<ProjectileData>(rBPName, m_prjBP);
}
sptr<const ModuleData> BlueprintLoader::getModuleSPtr(const std::string& rBPName) const
{
	return getData<ModuleData>(rBPName, m_modBP);
}
sptr<const WeaponData> BlueprintLoader::getWeaponSPtr(const std::string& rBPName) const
{
	return getData<WeaponData>(rBPName, m_wepBP);
}
sf::Color BlueprintLoader::loadColor(const Json::Value& root)
{
	sf::Color color;
	color.r = root[0].asInt();
	color.g = root[1].asInt();
	color.b = root[2].asInt();
	color.a = root[3].asInt();
	return color;
}
