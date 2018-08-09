#pragma once

#include "stdafx.hpp"
#include "Globals.hpp"
#include "JSON.hpp"
#include "NonCopyable.hpp"
#include "Debugging.hpp"
#include "ClassRegisterType.hpp"

struct ModuleData;
struct ProjectileData;
struct ChunkData;
struct WeaponData;
struct StaticDecorData;
struct DynamicDecorData;
struct Particles;
enum class UpgradeType;



/// Handles the loading of Blueprints from files.
class BlueprintLoader : NonCopyable
{
public:
	BlueprintLoader();
	virtual ~BlueprintLoader();
	///Loads blueprints from the given directory.
	void loadBlueprints(const String& rDir);
	///Returns a shared pointer to the ModuleData object created from the given blueprint.
	sptr<const ModuleData> getModuleSPtr(const String& rBPName) const;	
	///Returns a shared pointer to the Particle object created from the given blueprint.
	sptr<const Particles> getParticleSPtr(const String& rBPName) const;
	///Returns a shared pointer to the ChunkData object created from the given blueprint.
	sptr<const ChunkData> getChunkSPtr(const String& rBPName) const;
	///Returns a shared pointer to the WeaponData object created from the given blueprint.
	sptr<const WeaponData> getWeaponSPtr(const String& rBPName) const;
	///Returns a shared pointer to the ProjectileData object created from the given blueprint.
	sptr<const ProjectileData> getProjectileSPtr(const String& rBPName) const;
	///Returns a shared pointer to the StaticDecorData object created from the given blueprint.
	sptr<const StaticDecorData> getStaticDecorSPtr(const String& rBPName) const;
	///Returns a shared pointer to the DynamicDecorData object created from the given blueprint.
	sptr<const DynamicDecorData> getDynamicDecorSPtr(const String& rBPName) const;
	/// <summary>
	/// Upgrades the stats on a module or weapon blueprint. Returns what value it was upgraded to.
	/// </summary>
	int upgrade(String bpName, UpgradeType type, Team team);

	void addBp(String bpName, sptr<const ModuleData> bp);
	void addBp(String bpName, sptr<const ChunkData> bp);

protected:
private:

	template <typename T>
	void createTeamBlueprintCopies(std::map<String, sptr<T> >& blueprints)
	{
		std::map<String, sptr<T> > additionalBlueprints;

		const int numTeams = 2;
		const int startTeam = 1;

		for(int team = startTeam; team <= numTeams; ++team) // add teams
			for(auto it = blueprints.begin(); it != blueprints.end(); ++it)
			{
				String originalTitle = it->first;

				sptr<const T> data = getData<T>(originalTitle, blueprints);

				sptr<T> bp(data->clone());
				bp->setTeam((Team)team);
				additionalBlueprints[bp->title] = bp;
			}

		blueprints.insert(additionalBlueprints.begin(), additionalBlueprints.end());
	}

	/** IMPORTANT NOTE:
	/** Changing this file doesn't always trigger a recompile, you may need to manually do it!
	**/
	template <typename T>
	void storeFromFile(const String& title, const String& fullPath, std::map<String, sptr<T> >& blueprints)//load that blueprint
	{
		std::ifstream stream(fullPath, std::ifstream::binary);
		Json::Reader reader;
		Json::Value root;
		bool parsedSuccess = reader.parse(stream, root, false);

		if(parsedSuccess)
			blueprints[title] = loadData<T>(title, root);
		else
		{
			WARNING;
			Print << "\nParse Failed [" << fullPath << "].";
		}
	}
	template <typename T>
	sptr<T> loadData(const String& title, const Json::Value& root)
	{
		static_assert(std::is_base_of<BlueprintableData, T>::value, "T1 must derive from Base!");
		sptr<T> spMod;
		String ClassName = "garbage";
		GETJSON(ClassName);

		if(ClassName == "garbage")
		{
			WARNING;
			Print << "\nClassName not found in [" << title << "].";
		}


		//std::tuple<int, double> mappings;
		//const auto * instance = &std::get<int>(mappings);

		void* t = getGame()->getClassRegister();
		ClassRegisterType* reg = static_cast<ClassRegisterType*>(t);
		const T* instance = reg->deduce<T>(ClassName);

		if(instance != nullptr)//we derived it!
		{
			T* data = instance->clone();
			data->title = title;
			data->loadJson(root);
			spMod.reset(data);
		}
		else
		{
			WARNING;
			Print << "\nCan't Deduce Type [" << ClassName << "].";
		}

		return spMod;
	}
	template <typename T>
	sptr<const T> getData(const String& rBPName, const std::map<String, sptr<T> >& blueprints) const
	{
		auto it = blueprints.find(rBPName);

		if(it != blueprints.end())
			return it->second;
		else
		{
			WARNING;
			Print << "\nCouldnt find [" << rBPName << "].";
			return blueprints.begin()->second;
		}
	}

	std::map<String, sptr<ModuleData> > m_modBP;
	std::map<String, sptr<WeaponData> > m_wepBP;
	std::map<String, sptr<ChunkData> > m_cnkBP;
	std::map<String, sptr<ProjectileData> > m_prjBP;
	std::map<String, sptr<StaticDecorData> > m_sdcBP;
	std::map<String, sptr<DynamicDecorData> > m_ddcBP;
	std::map<String, sptr<Particles> > m_particlesBP;
};