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

using namespace std;

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
		return m_wepBP.begin()->second;
	}
}
/**===============**/
/**GET A BLUEPRINT**/






/**=================ROSTER====================**/
/**===========================================**/
void BlueprintLoader::storeRoster(const std::string& rDir)
{
	std::string directory = contentDir() + rDir;
	std::ifstream roster(directory + "roster.rst", std::ifstream::binary);

	Json::Reader readerRoster;
	Json::Value rootRoster;
	bool parsedSuccess = readerRoster.parse(roster, rootRoster, false);

	if(parsedSuccess)
	{

		const Json::Value weaponList = rootRoster["WeaponList"];
		for(auto it = weaponList.begin(); it != weaponList.end(); ++it)
		{
			std::string file = (directory + it->asString());
			storeWeapon(file);
		}

		const Json::Value bpList = rootRoster["ModuleList"];
		for(auto it = bpList.begin(); it != bpList.end(); ++it)
		{
			std::string file = (directory + it->asString());
			storeModule(file);
		}

		const Json::Value chunkList = rootRoster["ChunkList"];
		for(auto it = chunkList.begin(); it != chunkList.end(); ++it)
		{
			std::string file = (directory + it->asString());
			storeChunk(file);
		}

		const Json::Value prjList = rootRoster["ProjectileList"];
		for(auto it = prjList.begin(); it != prjList.end(); ++it)
		{
			std::string file = (directory + it->asString());
			storeProjectile(file);
		}

	}
	else
	{
		cout << "Couldn't Parse [" << directory + "roster.rst" << "]" << FILELINE;
		///ERRORLOG
	}
}
/**===========================================**/
/**=================ROSTER====================**/







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




//STILL BADD===========================================
//STILL BADD===========================================
//STILL BADD===========================================
sptr<const WeaponData> BlueprintLoader::loadWeapon(const Json::Value& root)//returns a weapon
{
	sptr<const WeaponData> spWep;

	if(root["WeaponType"].asString() == "Laser")
	{
		LaserWeaponData* pWep = new LaserWeaponData;
		if(!root["Copies"].isNull())
			*pWep = *dynamic_cast<const LaserWeaponData*>(getWeaponSPtr(root["Copies"].asString()).get());

		if(!root["BeamWidth"].isNull())
			pWep->beamWidth = root["BeamWidth"].asInt();
		if(!root["BeamColor"].isNull())
			pWep->beamColor = loadColor(root["BeamColor"]);
		if(!root["ShowTime"].isNull())
			pWep->showTime = root["ShowTime"].asFloat();
		if(!root["BeamStart"].isNull())
			pWep->beamComp.start = loadQuad(root["BeamStart"], pWep->beamComp.start);
		if(!root["BeamEnd"].isNull())
			pWep->beamComp.end = loadQuad(root["BeamEnd"], pWep->beamComp.end);
		if(!root["BeamMid"].isNull())
			static_cast<QuadComponentData>(pWep->beamComp) = loadQuad(root["BeamMid"], pWep->beamComp);


		inheritWeapon(root, pWep);
		spWep.reset(pWep);
	}
	else if(root["WeaponType"].asString() == "Ballistic")
	{
		BallisticWeaponData* pWep = new BallisticWeaponData;
		if(!root["Copies"].isNull())
			*pWep = *dynamic_cast<const BallisticWeaponData*>(getWeaponSPtr(root["Copies"].asString()).get());

		if(!root["ProjectileName"].isNull())
			pWep->projName = root["ProjectileName"].asString();

		inheritWeapon(root, pWep);
		spWep.reset(pWep);
	}
	else if(root["WeaponType"].asString() == "Missile")
	{
		MissileWeaponData* pWep = new MissileWeaponData;
		if(!root["Copies"].isNull())
			*pWep = *dynamic_cast<const MissileWeaponData*>(getWeaponSPtr(root["Copies"].asString()).get());

		if(!root["ProjectileName"].isNull())
			pWep->projName = root["ProjectileName"].asString();

		inheritWeapon(root, pWep);
		spWep.reset(pWep);
	}
	else
	{
		cout << "\n" << FILELINE;
		///ERROR LOG
	}

	return spWep;
}
void BlueprintLoader::inheritWeapon(const Json::Value& root, WeaponData* pWep)
{
	if(!root["EnergyConsumption"].isNull())
		pWep->ener = root["EnergyConsumption"].asFloat();
	if(!root["BallisticConsumption"].isNull())
		pWep->ball = root["BallisticConsumption"].asFloat();

	if(!root["Shots"].isNull())
		pWep->shots = root["Shots"].asInt();
	if(!root["Damage"].isNull())
		pWep->damage = root["Damage"].asInt();

	if(!root["StartSound"].isNull())
		pWep->startSound = loadSound(root["StartSound"], pWep->startSound);
	if(!root["ShotSound"].isNull())
		pWep->shotSound = loadSound(root["ShotSound"], pWep->shotSound);
	if(!root["EndSound"].isNull())
		pWep->endSound = loadSound(root["EndSound"], pWep->endSound);

	if(!root["ShotDelay"].isNull())
		pWep->shotDelay = root["ShotFrequency"].asFloat();
	if(!root["ReloadTime"].isNull())
		pWep->fireDelay = root["ReloadTime"].asFloat();

	if(!root["Range"].isNull())
		pWep->range = root["Range"].asFloat();

	if(!root["Collisions"].isNull())
		pWep->collisions = root["Collisions"].asInt();

	if(!root["WeaponSprite"].isNull())
		pWep->weaponQuad = loadQuad(root["WeaponSprite"], pWep->weaponQuad);
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
BodyComponentData BlueprintLoader::loadBodyComp(const Json::Value& root, const BodyComponentData& orig)
{
	BodyComponentData data(orig);

	if(!root["coords"].isNull())
	{
		data.coords.x = root["coords"][0].asFloat();
		data.coords.y = root["coords"][1].asFloat();
	}
	if(!root["rotation"].isNull())
		data.rotation = root["rotation"].asFloat();
	if(!root["isDynamic"].isNull())
		data.isDynamic = root["isDynamic"].asBool();
	if(!root["isBullet"].isNull())
		data.isBullet = root["isBullet"].asBool();
	if(!root["startAwake"].isNull())
		data.startAwake = root["startAwake"].asBool();

	return data;
}
HealthData BlueprintLoader::loadHealth(const Json::Value& root, const HealthData& orig)
{
	HealthData data(orig);

	if(!root["MaxHealth"].isNull())
		data.startMax = root["MaxHealth"].asInt();
	if(!root["Health"].isNull())
		data.startValue = root["Health"].asInt();
	if(!root["Armor"].isNull())
		data.armor = root["Armor"].asInt();

	return data;
}
/// <summary>
/// TODO USE INHERIT FUNCTION DO INHERIT GRAPHICSCOMPONENT data
/// </summary>
QuadComponentData BlueprintLoader::loadQuad(const Json::Value& root, const QuadComponentData& orig)
{
	QuadComponentData data(orig);

	if(!root["dimensions"].isNull())
	{
		data.dimensions.x = root["dimensions"][0].asInt();
		data.dimensions.y = root["dimensions"][1].asInt();
	}

	if(!root["permanentRot"].isNull())
		data.permanentRot = root["permanentRot"].asFloat();

	if(!root["center"].isNull())
	{
		data.center.x = root["center"][0].asInt();
		data.center.y = root["center"][1].asInt();
	}

	if(!root["texName"].isNull())
		data.texName = root["texName"].asString();
	if(!root["animSheetName"].isNull())
		data.animSheetName = root["animSheetName"].asString();
	if(!root["layer"].isNull())
		data.layer = ChooseLayer(root["layer"].asString());

	return data;
}
SoundData BlueprintLoader::loadSound(const Json::Value& root, const SoundData& orig)
{
	SoundData data(orig);

	if(!root["name"].isNull())
		data.name = root["name"].asString();
	if(!root["vol"].isNull())
		data.vol = root["vol"].asInt();
	if(!root["dropOff"].isNull())
		data.dropOff = root["dropOff"].asFloat();
	if(!root["minDist"].isNull())
		data.minDist = root["minDist"].asFloat();
	if(!root["relative"].isNull())
		data.relative = root["relative"].asBool();

	return data;
}
