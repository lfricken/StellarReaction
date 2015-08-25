#ifndef BLUEPRINTLOADER_HPP
#define BLUEPRINTLOADER_HPP

#include "stdafx.hpp"

#include "IOComponent.hpp"
#include "QuadComponent.hpp"
#include "Spinner.hpp"
#include "FixtureComponent.hpp"
#include "NetworkComponent.hpp"
#include "BodyComponent.hpp"
#include "Health.hpp"
#include "SoundData.hpp"


struct ShipModuleData;
struct ModuleData;
struct ProjectileData;
struct ChunkData;
struct WeaponData;

class BlueprintLoader
{
public:
	BlueprintLoader();
	virtual ~BlueprintLoader();


	/**GET A BLUEPRINT**/
	/**===============**/
	sptr<const ModuleData> getModuleSPtr(const std::string& rBPName) const;
	sptr<const ChunkData> getChunkSPtr(const std::string& rBPName) const;
	sptr<const WeaponData> getWeaponSPtr(const std::string& rBPName) const;
	sptr<const ProjectileData> getProjectileSPtr(const std::string& rBPName) const;
	/**===============**/
	/**GET A BLUEPRINT**/


	void storeRoster(const std::string& rDir);//load the roster blueprints into memory


	/**LOAD SPECIFIC FILES**/
	/**===================**/
	void storeModule(const std::string& rFile);
	void storeChunk(const std::string& rFile);
	void storeWeapon(const std::string& rFile);
	void storeProjectile(const std::string& rFile);
	/**===================**/
	/**LOAD SPECIFIC FILES**/



	/**LOAD MULTI PART DATA**/
	/**====================**/
	sptr<const ChunkData> loadChunk(const Json::Value& root);
	sptr<const ProjectileData> loadProjectile(const Json::Value& root);
protected:
private:
	sptr<const ModuleData> loadModule(const Json::Value& root);
	void inheritShipModule(const Json::Value& root, ShipModuleData* pSMod);
	void inheritModule(const Json::Value& root, ModuleData* pSMod);
	template <typename T>
	void copyModule(const Json::Value& root, T* pSMod)
	{
		if(!root["Copies"].isNull())
			*pSMod = *dynamic_cast<const T*>(getModuleSPtr(root["Copies"].asString()).get());
	}

	sptr<const WeaponData> loadWeapon(const Json::Value& root);
	void inheritWeapon(const Json::Value& root, WeaponData* pWep);
	/**====================**/
	/**LOAD MULTI PART DATA**/




	/**UNNAMED THINGS**/
	void insertModuleData(const Json::Value& root, std::vector<sptr<const ModuleData> >& rModData);
	void insertWeaponData(const Json::Value& root, sptr<const WeaponData>& rModData);




	/**LOAD ControlMPLE DATA**///data that doesnt inherit or anything
	/**================**/
	sf::Color loadColor(const Json::Value& root);
	BodyComponentData loadBodyComp(const Json::Value& root, const BodyComponentData& orig);
	IOComponentData loadIOComp(const Json::Value& root, const IOComponentData& orig);
	FixtureComponentData loadFixComp(const Json::Value& root, const FixtureComponentData& orig);
	NetworkComponentData loadNWComp(const Json::Value& root, const NetworkComponentData& orig);
	HealthData loadHealth(const Json::Value& root, const HealthData& orig);
	QuadComponentData loadQuad(const Json::Value& root, const QuadComponentData& orig);
	SoundData loadSound(const Json::Value& root, const SoundData& orig);

	template <typename T>
	PoolData<T> loadPool(const Json::Value& root, const PoolData<T>& orig)
	{
		PoolData<T> data(orig);

		if(!root["Max"].isNull())
			data.startMax = root["Max"].asInt();
		if(!root["Value"].isNull())
			data.startValue = root["Value"].asInt();

		return data;
	}
	/**================**/
	/**LOAD ControlMPLE DATA**///data that doesnt inherit or anything


	std::map<std::string, sptr<const ModuleData> > m_modBP;
	std::map<std::string, sptr<const WeaponData> > m_wepBP;
	std::map<std::string, sptr<const ChunkData> > m_cnkBP;
	std::map<std::string, sptr<const ProjectileData> > m_prjBP;
};

#endif // BLUEPRINTLOADER_HPP
