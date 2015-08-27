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
#include "ProjectileModule.hpp"
#include "LaserWeapon.hpp"
#include "BallisticWeapon.hpp"
#include "Projectile.hpp"

using namespace std;

BlueprintLoader::BlueprintLoader()
{

}
BlueprintLoader::~BlueprintLoader()
{

}
/**GET A BLUEPRINT**/
/**===============**/
sptr<const ChunkData> BlueprintLoader::getChunkSPtr(const std::string& rBPName) const
{
	auto it = m_cnkBP.find(rBPName);

	if (it != m_cnkBP.end())
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

	if (it != m_modBP.end())
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

	if (it != m_wepBP.end())
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
	std::ifstream roster(rDir + "roster.rst", std::ifstream::binary);
	Json::Reader readerRoster;
	Json::Value rootRoster;
	bool parsedSuccess = readerRoster.parse(roster, rootRoster, false);

	if (parsedSuccess)
	{

		const Json::Value weaponList = rootRoster["WeaponList"];
		for (auto it = weaponList.begin(); it != weaponList.end(); ++it)
		{
			std::string file = (rDir + it->asString());
			storeWeapon(file);
		}

		const Json::Value bpList = rootRoster["ModuleList"];
		for (auto it = bpList.begin(); it != bpList.end(); ++it)
		{
			std::string file = (rDir + it->asString());
			storeModule(file);
		}

		const Json::Value chunkList = rootRoster["ChunkList"];
		for (auto it = chunkList.begin(); it != chunkList.end(); ++it)
		{
			std::string file = (rDir + it->asString());
			storeChunk(file);
		}

		const Json::Value prjList = rootRoster["ProjectileList"];
		for(auto it = prjList.begin(); it != prjList.end(); ++it)
		{
			std::string file = (rDir + it->asString());
			storeProjectile(file);
		}

	}
	else
	{
		cout << "Couldn't Parse [" << rDir + "roster.rst" << "]" << FILELINE;
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

	if (parsedSuccess)
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

	if (parsedSuccess)
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

	if (parsedSuccess)
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
/**===================**/
/**LOAD SPECIFIC FILES**/







/**LOAD MULTI PART DATA**/
/**====================**/
sptr<const ChunkData> BlueprintLoader::loadChunk(const Json::Value& root)//returns data based on the Json stuff you pass
{
	sptr<const ChunkData> spCnk;

	if (root["ClassName"].asString() == "Chunk")
	{
		ChunkData* pCnk = new ChunkData;

		if (!root["Copies"].isNull())
			*pCnk = *dynamic_cast<const ChunkData*>(getChunkSPtr(root["Copies"].asString()).get());

		if (!root["IO"].isNull())
			pCnk->ioComp = loadIOComp(root["IO"], pCnk->ioComp);
		if (!root["Network"].isNull())
			pCnk->nwComp = loadNWComp(root["Network"], pCnk->nwComp);
		if (!root["Body"].isNull())
			pCnk->bodyComp = loadBodyComp(root["Body"], pCnk->bodyComp);
		if (!root["Missiles"].isNull())
			pCnk->missileData = loadPool<Missiles>(root["Missiles"], pCnk->missileData);
		if (!root["Ballistic"].isNull())
			pCnk->ballisticData = loadPool<Ballistic>(root["Ballistic"], pCnk->ballisticData);
		if (!root["Energy"].isNull())
			pCnk->energyData = loadPool<Energy>(root["Energy"], pCnk->energyData);
		if (!root["Zoom"].isNull())
			pCnk->zoomData = loadPool<float>(root["Zoom"], pCnk->zoomData);

		//TODO - remove
		//check hull sprite
		//if (!root["Hull_Sprite"].isNull())
		//{
		//	for (auto it = root["Hull_Sprite"].begin(); it != root["Hull_Sprite"].end(); ++it)
		//	{
		//		pCnk->hullData.push_back(loadQuad(*it, QuadComponentData()));
		//	}
		//}

		//Evan - load hull sprite, afterburner and afterburner_thrust anims
		if (!root["Hull_Sprite"].isNull())
		{
			pCnk->hullSpriteData = loadQuad(root["Hull_Sprite"], QuadComponentData());
		}
		if (!root["Afterburner_Sprite"].isNull())
		{
			pCnk->afterburnerSpriteData = loadQuad(root["Afterburner_Sprite"], QuadComponentData());
		}
		if (!root["Afterburner_Thrust_Sprite"].isNull())
		{
			pCnk->afterburnerThrustSpriteData = loadQuad(root["Afterburner_Thrust_Sprite"], QuadComponentData());
		}

		if(!root["Modules"].isNull())
			insertModuleData(root["Modules"], pCnk->moduleData);

		spCnk.reset(pCnk);
	}
	else
	{
		cout << "\n" << FILELINE;
		///ERROR LOG
	}
	return spCnk;
}


/// <summary>
/// Take a json projectile and load it.
/// </summary>
sptr<const ProjectileData> BlueprintLoader::loadProjectile(const Json::Value& root)
{
	sptr<const ProjectileData> spPrj;

	if(root["ClassName"].asString() == "Projectile")
	{
		ProjectileData* pPrj = new ProjectileData;

		if(!root["Copies"].isNull())
			*pPrj = *dynamic_cast<const ProjectileData*>(getProjectileSPtr(root["Copies"].asString()).get());

		pPrj->title = root["Title"].asString();

		if(!root["Body"].isNull())
			pPrj->body = loadBodyComp(root["Body"], pPrj->body);

		if(!root["Modules"].isNull())
			insertModuleData(root["Modules"], pPrj->moduleData);

		spPrj.reset(pPrj);
	}
	else
	{
		cout << "\n" << FILELINE;
		///ERROR LOG
	}
	return spPrj;
}



sptr<const WeaponData> BlueprintLoader::loadWeapon(const Json::Value& root)//returns a weapon
{
	sptr<const WeaponData> spWep;

	if (root["WeaponType"].asString() == "Laser")
	{
		LaserWeaponData* pWep = new LaserWeaponData;
		if (!root["Copies"].isNull())
			*pWep = *dynamic_cast<const LaserWeaponData*>(getWeaponSPtr(root["Copies"].asString()).get());

		if (!root["BeamWidth"].isNull())
			pWep->beamWidth = root["BeamWidth"].asInt();
		if (!root["BeamColor"].isNull())
			pWep->beamColor = loadColor(root["BeamColor"]);
		if (!root["ShowTime"].isNull())
			pWep->showTime = root["ShowTime"].asFloat();
		if (!root["BeamStart"].isNull())
			pWep->beamComp.start = loadQuad(root["BeamStart"], pWep->beamComp.start);
		if (!root["BeamEnd"].isNull())
			pWep->beamComp.end = loadQuad(root["BeamEnd"], pWep->beamComp.end);
		if (!root["BeamMid"].isNull())
			static_cast<QuadComponentData>(pWep->beamComp) = loadQuad(root["BeamMid"], pWep->beamComp);


		inheritWeapon(root, pWep);
		spWep.reset(pWep);
	}
	else if (root["WeaponType"].asString() == "Ballistic")
	{
		BallisticWeaponData* pWep = new BallisticWeaponData;
		if(!root["Copies"].isNull())
			*pWep = *dynamic_cast<const BallisticWeaponData*>(getWeaponSPtr(root["Copies"].asString()).get());

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
	if (!root["EnergyConsumption"].isNull())
		pWep->ener = root["EnergyConsumption"].asFloat();
	if (!root["BallisticConsumption"].isNull())
		pWep->ball = root["BallisticConsumption"].asFloat();

	if (!root["Shots"].isNull())
		pWep->shots = root["Shots"].asInt();
	if (!root["Damage"].isNull())
		pWep->damage = root["Damage"].asInt();

	if(!root["StartSound"].isNull())
		pWep->startSound = loadSound(root["StartSound"], pWep->startSound);
	if(!root["ShotSound"].isNull())
		pWep->shotSound = loadSound(root["ShotSound"], pWep->shotSound);
	if(!root["EndSound"].isNull())
		pWep->endSound = loadSound(root["EndSound"], pWep->endSound);

	if (!root["ShotDelay"].isNull())
		pWep->shotDelay = root["ShotFrequency"].asFloat();
	if (!root["ReloadTime"].isNull())
		pWep->fireDelay = root["ReloadTime"].asFloat();

	if (!root["Range"].isNull())
		pWep->range = root["Range"].asFloat();

	if(!root["Collisions"].isNull())
		pWep->collisions = root["Collisions"].asInt();

	if (!root["WeaponSprite"].isNull())
		pWep->weaponQuad = loadQuad(root["WeaponSprite"], pWep->weaponQuad);
}






sptr<const ModuleData> BlueprintLoader::loadModule(const Json::Value& root)//returns Data based on the Json stuff you pass it FOR ACTUAL MODULES
{
	sptr<const ModuleData> spMod;

	/**=================**/
	/**==== MODULES ====**/
	/**=================**/
	if (root["ClassName"].asString() == "Module")
	{
		ModuleData* pSMod = new ModuleData;
		copyModule<ModuleData>(root, pSMod);
		inheritModule(root, pSMod);
		spMod.reset(pSMod);
	}
	else if (root["ClassName"].asString() == "Sensor")
	{
		SensorData* pSMod = new SensorData;
		copyModule<SensorData>(root, pSMod);
		inheritModule(root, pSMod);
		spMod.reset(pSMod);
	}
	else if (root["ClassName"].asString() == "ShipModule")
	{
		ShipModuleData* pSMod = new ShipModuleData;
		inheritModule(root, pSMod);
		spMod.reset(pSMod);
	}


	/**======================**/
	/**==== SHIP MODULES ====**/
	/**======================**/
	else if (root["ClassName"].asString() == "Turret")
	{
		TurretData* pSMod = new TurretData;
		copyModule<TurretData>(root, pSMod);

		if (!root["StartEmpty"].isNull())
			pSMod->startEmpty = root["StartEmpty"].asBool();
		if (!root["Weapon"].isNull())
			insertWeaponData(root["Weapon"], pSMod->startWep);

		inheritShipModule(root, pSMod);
		spMod.reset(pSMod);
	}
	else if (root["ClassName"].asString() == "Plating")
	{
		PlatingData* pSMod = new PlatingData;
		copyModule<PlatingData>(root, pSMod);

		inheritShipModule(root, pSMod);
		spMod.reset(pSMod);
	}
	else if (root["ClassName"].asString() == "Radar")
	{
		RadarData* pSMod = new RadarData;
		copyModule<RadarData>(root, pSMod);

		if (!root["RadarStrength"].isNull())
			pSMod->zoomAddition = root["RadarStrength"].asFloat();

		inheritShipModule(root, pSMod);
		spMod.reset(pSMod);
	}
	else if (root["ClassName"].asString() == "Thruster")
	{
		ThrusterData* pSMod = new ThrusterData;
		copyModule<ThrusterData>(root, pSMod);

		if (!root["Force"].isNull())
			pSMod->force = root["Force"].asFloat();
		if (!root["Torque"].isNull())
			pSMod->torque = root["Torque"].asFloat();
		if (!root["EnergyConsumption"].isNull())
			pSMod->energyConsumption = root["EnergyConsumption"].asFloat();

		inheritShipModule(root, pSMod);
		spMod.reset(pSMod);
	}
	else if (root["ClassName"].asString() == "Capacitor")
	{
		CapacitorData* pSMod = new CapacitorData;
		copyModule<CapacitorData>(root, pSMod);

		if (!root["EnergyCapacity"].isNull())
			pSMod->storage = root["EnergyCapacity"].asFloat();

		inheritShipModule(root, pSMod);
		spMod.reset(pSMod);
	}
	else if (root["ClassName"].asString() == "Reactor")
	{
		ReactorData* pSMod = new ReactorData;
		copyModule<ReactorData>(root, pSMod);

		if (!root["EnergyProduction"].isNull())
			pSMod->rate = root["EnergyProduction"].asFloat();

		inheritShipModule(root, pSMod);
		spMod.reset(pSMod);
	}
	else if (root["ClassName"].asString() == "Projectile")
	{
		ProjectileModuleData* pSMod = new ProjectileModuleData;
		copyModule<ProjectileModuleData>(root, pSMod);

		if(!root["BaseSprite"].isNull())
			pSMod->baseDecor = loadQuad(root["BaseSprite"], pSMod->baseDecor);

		inheritModule(root, pSMod);
		spMod.reset(pSMod);
	}
	else
	{
		cout << "\n" << "Couldn't Find [" << root["ClassName"].asString() << "]" << FILELINE;
		///ERROR LOG
	}

	return spMod;
}
void BlueprintLoader::inheritShipModule(const Json::Value& root, ShipModuleData* pSMod)//do things that they all have in common
{
	/**INHERIT**/
	if (!root["Inherits"].isNull())
		*static_cast<ShipModuleData*>(pSMod) = *static_cast<const ShipModuleData*>(getModuleSPtr(root["Inherits"].asString()).get());

	/**OVERWRITES**/
	if (!root["Defense"].isNull())
		pSMod->health = loadHealth(root["Defense"], pSMod->health);
	if (!root["BaseSprite"].isNull())
		pSMod->baseDecor = loadQuad(root["BaseSprite"], pSMod->baseDecor);

	/**ParentData**/
	inheritModule(root, pSMod);
}
void BlueprintLoader::inheritModule(const Json::Value& root, ModuleData* pSMod)//do things that they all have in common
{
	if (!root["Inherits"].isNull())
		*static_cast<ModuleData*>(pSMod) = *static_cast<const ModuleData*>(getModuleSPtr(root["Inherits"].asString()).get());
	/**OVERWRITES**/
	if (!root["IO"].isNull())
		pSMod->ioComp = loadIOComp(root["IO"], pSMod->ioComp);
	if (!root["Physics"].isNull())
		pSMod->fixComp = loadFixComp(root["Physics"], pSMod->fixComp);
	if (!root["Network"].isNull())
		pSMod->nwComp = loadNWComp(root["Network"], pSMod->nwComp);
}
/**====================**/
/**LOAD MULTI PART DATA**/













/// <summary>
/// Load a list of modules. Even if they are inlined
/// </summary>
void BlueprintLoader::insertModuleData(const Json::Value& root, std::vector<sptr<const ModuleData> >& rModData)
{
	sptr<ModuleData> spMod;
	for (auto it = root.begin(); it != root.end(); ++it)
	{
		if(!(*it)["Title"].isNull() && (*it)["ClassName"].isNull())//from title
		{
			spMod.reset(getModuleSPtr((*it)["Title"].asString())->clone());

			spMod->fixComp.offset.x = (*it)["Position"][0].asFloat();
			spMod->fixComp.offset.y = (*it)["Position"][1].asFloat();
		}
		else if(!(*it)["ClassName"].isNull())//from inline
		{
			spMod.reset(loadModule(*it)->clone());
		}
		else
		{
			cout << "\n" << FILELINE;
			///ERROR LOG
		}

		rModData.push_back(spMod);
	}
}
void BlueprintLoader::insertWeaponData(const Json::Value& root, sptr<const WeaponData>& rModData)
{
	sptr<WeaponData> spWep;

	if (!root["Title"].isNull() && root["WeaponType"].isNull())
	{
		spWep.reset(getWeaponSPtr(root["Title"].asString())->clone());
	}
	else if (!root["WeaponType"].isNull())
	{
		spWep.reset(loadWeapon(root)->clone());
	}
	else
	{
		cout << "\n" << FILELINE;
		///ERROR LOG
	}

	rModData = spWep;
}









/**LOAD ControlMPLE DATA**///data that doesnt inherit or anything
/**================**/
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

	if (!root["coords"].isNull())
	{
		data.coords.x = root["coords"][0].asFloat();
		data.coords.y = root["coords"][1].asFloat();
	}
	if (!root["rotation"].isNull())
		data.rotation = root["rotation"].asFloat();
	if (!root["isDynamic"].isNull())
		data.isDynamic = root["isDynamic"].asBool();
	if (!root["isBullet"].isNull())
		data.isBullet = root["isBullet"].asBool();
	if (!root["startAwake"].isNull())
		data.startAwake = root["startAwake"].asBool();

	return data;
}
IOComponentData BlueprintLoader::loadIOComp(const Json::Value& root, const IOComponentData& orig)
{
	IOComponentData data(orig);

	if (!root["name"].isNull())
		data.name = root["name"].asString();

	if (!root["courierList"].isNull())
	{
		const Json::Value courierList = root["courierList"];
		for (auto it = courierList.begin(); it != courierList.end(); ++it)
		{
			Courier c;

			string target = (*it)["message"]["target"].asString();
			string command = (*it)["message"]["command"].asString();

			sf::Packet packData;
			const Json::Value dataList = (*it)["message"]["data"];
			if (dataList.size() % 2 == 0)//if it's divisible by two
				for (auto it = dataList.begin(); it != dataList.end(); ++it)
				{
					string type = it->asString();

					++it;
					if (type == "bool")
						packData << it->asBool();
					if (type == "int")
						packData << it->asInt();
					if (type == "float")
						packData << it->asFloat();
					if (type == "string")
						packData << it->asString();
				}
			else
			{
				cout << "\n" << FILELINE;
				///ERROR LOG
			}

			float delay = (*it)["message"]["delay"].asFloat();
			bool sendValue = (*it)["message"]["sendValue"].asBool();
			c.message.reset(target, command, packData, delay, sendValue);

			EventType event = ChooseEvent((*it)["condition"]["event"].asString());
			int value = (*it)["condition"]["value"].asInt();
			char comp = (*it)["condition"]["comparator"].asString()[0];
			bool repeatable = (*it)["condition"]["repeatable"].asBool();
			c.condition.reset(event, value, comp, repeatable);

			data.courierList.push_back(c);
		}
	}

	return data;
}
FixtureComponentData BlueprintLoader::loadFixComp(const Json::Value& root, const FixtureComponentData& orig)
{
	FixtureComponentData data(orig);
	if (!root["offset"].isNull())
	{
		data.offset.x = root["offset"][0].asFloat();
		data.offset.y = root["offset"][1].asFloat();
	}

	if (!root["shape"].isNull())
	{
		string temp = root["shape"].asString();
		if (temp == "rectangle")
			data.shape = leon::Shape::Rectangle;
		else if (temp == "circle")
			data.shape = leon::Shape::Circle;
		else
		{
			cout << "\n" << FILELINE;
			data.shape = leon::Shape::Circle;
		}
	}

	if (!root["size"].isNull())
	{
		data.size.x = root["size"][0].asFloat();
		data.size.y = root["size"][1].asFloat();
	}

	if (!root["density"].isNull())
		data.density = root["density"].asFloat();
	if (!root["friction"].isNull())
		data.friction = root["friction"].asFloat();
	if (!root["restitution"].isNull())
		data.restitution = root["restitution"].asFloat();
	if (!root["isSensor"].isNull())
		data.isSensor = root["isSensor"].asBool();

	if (!root["colCat"].isNull())
		data.colCategory = ChooseCategory(root["colCat"].asString());
	if (!root["colMask"].isNull())
		data.colMask = ChooseMask(root["colMask"].asString());

	return data;
}
NetworkComponentData BlueprintLoader::loadNWComp(const Json::Value& root, const NetworkComponentData& orig)
{
	NetworkComponentData data(orig);


	return data;
}
HealthData BlueprintLoader::loadHealth(const Json::Value& root, const HealthData& orig)
{
	HealthData data(orig);

	if (!root["MaxHealth"].isNull())
		data.startMax = root["MaxHealth"].asInt();
	if (!root["Health"].isNull())
		data.startValue = root["Health"].asInt();
	if (!root["Armor"].isNull())
		data.armor = root["Armor"].asInt();

	return data;
}
/// <summary>
/// TODO USE INHERIT FUNCTION DO INHERIT GRAPHICSCOMPONENT data
/// </summary>
QuadComponentData BlueprintLoader::loadQuad(const Json::Value& root, const QuadComponentData& orig)
{
	QuadComponentData data(orig);

	if (!root["dimensions"].isNull())
	{
		data.dimensions.x = root["dimensions"][0].asInt();
		data.dimensions.y = root["dimensions"][1].asInt();
	}

	if (!root["permanentRot"].isNull())
		data.permanentRot = root["permanentRot"].asFloat();

	if (!root["center"].isNull())
	{
		data.center.x = root["center"][0].asInt();
		data.center.y = root["center"][1].asInt();
	}

	if (!root["texName"].isNull())
		data.texName = root["texName"].asString();
	if (!root["animSheetName"].isNull())
		data.animSheetName = root["animSheetName"].asString();
	if (!root["layer"].isNull())
		data.layer = ChooseLayer(root["layer"].asString());

	return data;
}
SoundData BlueprintLoader::loadSound(const Json::Value& root, const SoundData& orig)
{
	SoundData data(orig);

	if (!root["name"].isNull())
		data.name = root["name"].asString();
	if (!root["vol"].isNull())
		data.vol = root["vol"].asInt();
	if (!root["dropOff"].isNull())
		data.dropOff = root["dropOff"].asFloat();
	if (!root["minDist"].isNull())
		data.minDist = root["minDist"].asFloat();
	if (!root["relative"].isNull())
		data.relative = root["relative"].asBool();

	return data;
}
