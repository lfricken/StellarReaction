#include "BlueprintLoader.hpp"

#include "Directory.hpp"

#include "ModuleRegistration.hpp"
#include "WeaponRegistration.hpp"
#include "ChunkRegistration.hpp"
#include "ProjectileRegistration.hpp"
#include "DecorRegistration.hpp"

using namespace std;

BlueprintLoader::BlueprintLoader()
{

}
BlueprintLoader::~BlueprintLoader()
{

}
void BlueprintLoader::loadBlueprints(const std::string& rDir)
{
	vector<pair<string, string> > weapons = game.getDir().getAllFiles(rDir + "weapons", ".bp");
	vector<pair<string, string> > modules = game.getDir().getAllFiles(rDir + "modules", ".bp");
	vector<pair<string, string> > chunks = game.getDir().getAllFiles(rDir + "chunks", ".bp");
	vector<pair<string, string> > projectiles = game.getDir().getAllFiles(rDir + "projectiles", ".bp");
	vector<pair<string, string> > static_decors = game.getDir().getAllFiles(rDir + "decorations/static", ".bp");
	vector<pair<string, string> > dynamic_decors = game.getDir().getAllFiles(rDir + "decorations/dynamic", ".bp");



	for(auto it = weapons.begin(); it != weapons.end(); ++it)
		storeData<WeaponData>(it->first, it->second, m_wepBP);

	for(auto it = modules.begin(); it != modules.end(); ++it)
		storeData<ModuleData>(it->first, it->second, m_modBP);

	for(auto it = chunks.begin(); it != chunks.end(); ++it)
		storeData<ChunkData>(it->first, it->second, m_cnkBP);

	for(auto it = projectiles.begin(); it != projectiles.end(); ++it)
		storeData<ProjectileData>(it->first, it->second, m_prjBP);

	for (auto it = static_decors.begin(); it != static_decors.end(); ++it)
		storeData<StaticDecorData>(it->first, it->second, m_sdcBP);
	
	for (auto it = dynamic_decors.begin(); it != dynamic_decors.end(); ++it)
		storeData<DynamicDecorData>(it->first, it->second, m_ddcBP);
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
sptr<const StaticDecorData> BlueprintLoader::getStaticDecorSPtr(const std::string& rBPName) const
{
	return getData<StaticDecorData>(rBPName, m_sdcBP);
}
sptr<const DynamicDecorData> BlueprintLoader::getDynamicDecorSPtr(const std::string& rBPName) const
{
	return getData<DynamicDecorData>(rBPName, m_ddcBP);
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
