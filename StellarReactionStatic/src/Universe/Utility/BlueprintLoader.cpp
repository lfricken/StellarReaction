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
void BlueprintLoader::storeRoster(const std::string& rDir)
{
	vector<string> weapons = game.getDir().getAllFiles(rDir + "weapons", ".bp");
	vector<string> modules = game.getDir().getAllFiles(rDir + "modules", ".bp");
	vector<string> chunks = game.getDir().getAllFiles(rDir + "chunks", ".bp");
	vector<string> projectiles = game.getDir().getAllFiles(rDir + "projectiles", ".bp");

	for(auto it = weapons.begin(); it != weapons.end(); ++it)
		storeWeapon(*it);

	for(auto it = modules.begin(); it != modules.end(); ++it)
		storeModule(*it);

	for(auto it = chunks.begin(); it != chunks.end(); ++it)
		storeChunk(*it);

	for(auto it = projectiles.begin(); it != projectiles.end(); ++it)
		storeProjectile(*it);
}
sptr<const ChunkData> BlueprintLoader::getChunkSPtr(const std::string& rBPName) const
{
	auto it = m_cnkBP.find(rBPName);

	if(it != m_cnkBP.end())
		return it->second;
	else
	{
		cout << "\nCouldnt find [" << rBPName << "]." << FILELINE;
		///ERROR LOG
		return m_cnkBP.begin()->second;
	}
}
sptr<const ProjectileData> BlueprintLoader::getProjectileSPtr(const std::string& rBPName) const
{
	auto it = m_prjBP.find(rBPName);

	if(it != m_prjBP.end())
		return it->second;
	else
	{
		cout << "\nCouldnt find [" << rBPName << "]." << FILELINE;
		///ERROR LOG
		return m_prjBP.begin()->second;
	}
}
sptr<const ModuleData> BlueprintLoader::getModuleSPtr(const std::string& rBPName) const
{
	auto it = m_modBP.find(rBPName);

	if(it != m_modBP.end())
		return it->second;
	else
	{
		cout << "\nCouldnt find [" << rBPName << "]." << FILELINE;
		///ERROR LOG
		return m_modBP.begin()->second;
	}
}
sptr<const WeaponData> BlueprintLoader::getWeaponSPtr(const std::string& rBPName) const
{
	auto it = m_wepBP.find(rBPName);

	if(it != m_wepBP.end())
		return it->second;
	else
	{
		cout << "\nCouldnt find [" << rBPName << "]." << FILELINE;
		///ERROR LOG
		return sptr<const WeaponData>(NULL);
	}
}
void BlueprintLoader::storeModule(const std::string& rFile)//load that blueprint
{
	std::ifstream stream(rFile, std::ifstream::binary);
	Json::Reader reader;
	Json::Value root;
	bool parsedSuccess = reader.parse(stream, root, false);

	if(parsedSuccess)
	{
		m_modBP[root["Title"].asString()] = loadModule(root);
	}
	else
	{
		cout << "\n" << FILELINE;
		///ERROR LOG
	}
}
void BlueprintLoader::storeChunk(const std::string& rFile)//load that blueprint
{
	std::ifstream stream(rFile, std::ifstream::binary);
	Json::Reader reader;
	Json::Value root;
	bool parsedSuccess = reader.parse(stream, root, false);

	if(parsedSuccess)
	{
		m_cnkBP[root["Title"].asString()] = loadChunk(root);
	}
	else
	{
		cout << "\n" << FILELINE;
		///ERROR LOG
	}
}
void BlueprintLoader::storeProjectile(const std::string& rFile)//load that blueprint
{
	std::ifstream stream(rFile, std::ifstream::binary);
	Json::Reader reader;
	Json::Value root;
	bool parsedSuccess = reader.parse(stream, root, false);

	if(parsedSuccess)
	{
		m_prjBP[root["Title"].asString()] = loadProjectile(root);
	}
	else
	{
		cout << "\n" << FILELINE;
		///ERROR LOG
	}
}
void BlueprintLoader::storeWeapon(const std::string& rFile)
{
	std::ifstream stream(rFile, std::ifstream::binary);
	Json::Reader reader;
	Json::Value root;
	bool parsedSuccess = reader.parse(stream, root, false);

	if(parsedSuccess)
	{
		m_wepBP[root["Title"].asString()] = loadWeapon(root);
	}
	else
	{
		cout << "\nParse Failure on file [" << rFile << "]";
		cout << "\n" << FILELINE;
		///ERROR LOG
	}
}
sptr<ChunkData> BlueprintLoader::loadChunk(const Json::Value& root)//returns data based on the Json stuff you pass
{
	sptr<ChunkData> spMod;
	const std::string title = root["ClassName"].asString();
	const ChunkData* instance = Deduce<ChunkData>::from(title);

	if(instance != NULL)//we derived it!
	{
		ChunkData* data = instance->clone();
		data->loadJson(root);
		spMod.reset(data);
	}
	else
		cout << "\n" << "Couldn't Find [" << title << "]" << FILELINE;

	return spMod;
}
sptr<const ProjectileData> BlueprintLoader::loadProjectile(const Json::Value& root)
{
	sptr<const ProjectileData> spMod;
	const std::string title = root["ClassName"].asString();
	const ProjectileData* instance = Deduce<ProjectileData>::from(title);

	if(instance != NULL)//we derived it!
	{
		ProjectileData* data = instance->clone();
		data->loadJson(root);
		spMod.reset(data);
	}
	else
		cout << "\n" << "Couldn't Find [" << title << "]" << FILELINE;

	return spMod;
}
sptr<const ModuleData> BlueprintLoader::loadModule(const Json::Value& root)
{
	sptr<const ModuleData> spMod;
	const std::string title = root["ClassName"].asString();
	const ModuleData* instance = Deduce<ModuleData>::from(title);

	if(instance != NULL)//we derived it!
	{
		ModuleData* data = instance->clone();
		data->loadJson(root);
		spMod.reset(data);
	}
	else
		cout << "\n" << "Couldn't Find [" << title << "]" << FILELINE;

	return spMod;
}
sptr<const WeaponData> BlueprintLoader::loadWeapon(const Json::Value& root)//returns a weapon
{
	sptr<WeaponData> spMod;
	const std::string title = root["ClassName"].asString();
	const WeaponData* instance = Deduce<WeaponData>::from(title);

	if(instance != NULL)//we derived it!
	{
		WeaponData* data = instance->clone();
		data->loadJson(root);
		spMod.reset(data);
	}
	else
		cout << "\n" << "Couldn't Find [" << title << "]" << FILELINE;

	return spMod;
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
