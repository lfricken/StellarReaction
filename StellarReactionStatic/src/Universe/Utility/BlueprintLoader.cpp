#include "BlueprintLoader.hpp"

#include "Module.hpp"
#include "ShipModule.hpp"
#include "Thruster.hpp"
#include "Capacitor.hpp"
#include "Reactor.hpp"
#include "Chunk.hpp"
#include "Sensor.hpp"
#include "Radar.hpp"
#include "Plating.hpp"
#include "Turret.hpp"
#include "Stealth.hpp"
#include "ProjectileModule.hpp"
#include "LaserWeapon.hpp"
#include "BallisticWeapon.hpp"
#include "Projectile.hpp"
#include "Missile.hpp"
#include "CaptureArea.hpp"
#include "MissileWeapon.hpp"


#include "ModuleRegistration.hpp"
#include "WeaponRegistration.hpp"
#include "ChunkRegistration.hpp"
#include "ProjectileRegistration.hpp"

#undef RegisterClass

#include <Windows.h>



// IterateFiles.cpp : Defines the entry point for the console application.
//




using namespace std;

string getCurrentPath()
{
	char path[_MAX_PATH];
	_fullpath(path, ".\\", _MAX_PATH);

	string pathString = string(path);

	// Get rid of \\Debug
	size_t debugString = pathString.find("\\Debug");

	// We want the \\ so +2 of the index given by \\Debug
	return pathString.substr(0, debugString + 1);
}

/// <summary>
/// Input a filename to validate whether or not it's a blueprint file.
/// </summary>
boolean isBluePrint(string fileName)
{
	string extension = fileName.substr(fileName.find_last_of(".") + 1);

	return strcmp(extension.c_str(), "bp") == 0;
}

string convertPath(string original, const string& rDir)
{
	size_t relative = original.find("StellarReaction\\blueprints") + string("StellarReaction\\blueprints").size() + 1;

	string relativePath = contentDir() + rDir + original.substr(relative, original.size());

	string::size_type n = 0;

	while((n = relativePath.find("\\", n)) != string::npos)
	{
		relativePath.replace(n, 1, "/");
		n += 1;
	}

	return relativePath;
}

/// <summary>
/// Heavily inspired by https://jbarkes.wordpress.com/2009/12/09/iterate-directories-files-in-c/
///
/// Iterates through a given folder and retrieves all th
/// </summary>
vector<string> getBluePrints(string startPath, const string& rDir)
{
	vector<string> blueprints = vector<string>();

	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATAA fData;
	string path;
	string filePath;

	stack<string> folders;
	folders.push(startPath);

	while(!folders.empty())
	{
		path = folders.top() + "\\" + "*";
		folders.pop();

		hFind = FindFirstFileA(path.c_str(), &fData);


		// As long as the root folder is not invalid, go crazy.
		if(hFind != INVALID_HANDLE_VALUE)
		{
			// As long as there are more files/folders, keep iterating.
			while(FindNextFileA(hFind, &fData) != 0)
			{
				// First get rid of the folders that are a reference to itself or a higher node (ie .\ and ..\)
				if(strcmp(fData.cFileName, ".") != 0 && strcmp(fData.cFileName, "..") != 0)
				{
					filePath = path.substr(0, path.size() - 1) + fData.cFileName;
					// If the current "File" is actually a folder, add it to the stack of folders to be iterated later.
					if(fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						folders.push(filePath);
					}
					// If the file has a .bp extension, add to blueprints.
					else if(isBluePrint(fData.cFileName))
					{
						string bp = convertPath(filePath, rDir);
						blueprints.push_back(bp);
					}
				}
			}
		}
	}

	return blueprints;
}

vector<string> getRoster(string extension, const string& rDir)
{
	string fullpath = getCurrentPath() + rDir + extension;//getCurrentPath() + "..\\" + "blueprints\\" + extension;
	// Replace all / with \\ 
	string::size_type n = 0;

	while ((n = fullpath.find("/", n)) != string::npos)
	{
		fullpath.replace(n, 1, "\\");
		n += 1;
	}

	return getBluePrints(fullpath, rDir);
}



void BlueprintLoader::storeRoster(const std::string& rDir)
{
	vector<string> weapons = getRoster("weapon", rDir);
	vector<string> modules = getRoster("modules", rDir);
	vector<string> chunks = getRoster("chunks", rDir);
	vector<string> projectiles = getRoster("projectiles", rDir);

	for(auto it = weapons.begin(); it != weapons.end(); ++it)
		storeWeapon(*it);

	for(auto it = modules.begin(); it != modules.end(); ++it)
		storeModule(*it);

	for(auto it = chunks.begin(); it != chunks.end(); ++it)
		storeChunk(*it);

	for(auto it = projectiles.begin(); it != projectiles.end(); ++it)
		storeProjectile(*it);
}





BlueprintLoader::BlueprintLoader()
{
	//StealthData data;
}
BlueprintLoader::~BlueprintLoader()
{

}
/**GET A BLUEPRINT**/
/**===============**/
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
/**===============**/
/**GET A BLUEPRINT**/

/**LOAD SPECIFIC FILES**/
/**===================**/
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



//LOADING =====================================
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
