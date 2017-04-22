#pragma once

#include "stdafx.hpp"
#include "Globals.hpp"
#include "JSON.hpp"
#include "NonCopyable.hpp"
#include "Debugging.hpp"

struct ModuleData;
struct ProjectileData;
struct ChunkData;
struct WeaponData;
struct StaticDecorData;
struct DynamicDecorData;
struct Particles;




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
protected:
private:

	/** IMPORTANT NOTE:
	/** Changing this file doesn't always trigger a recompile, you may need to manually do it!
	**/
	template <typename T>
	void storeData(const String& title, const String& fullPath, std::map<String, sptr<const T> >& blueprints)//load that blueprint
	{
		std::ifstream stream(fullPath, std::ifstream::binary);
		Json::Reader reader;
		Json::Value root;
		bool parsedSuccess = reader.parse(stream, root, false);

		if(parsedSuccess)
			blueprints[title] = loadData<T>(title, root);
		else
			std::cout << "\nParse Failed [" << fullPath << "].\n" << FILELINE;
	}
	template <typename T>
	sptr<const T> loadData(const String& title, const Json::Value& root)
	{
		static_assert(std::is_base_of<BlueprintData, T>::value, "T1 must derive from Base");
		sptr<const T> spMod;
		String ClassName = "garbage";
		GETJSON(ClassName);

		if(ClassName == "garbage")
			std::cout << "\nClassName not found in [" << title << "].\n" << FILELINE;

		const T* instance = Deduce<T>::from(ClassName);

		if(instance != NULL)//we derived it!
		{
			T* data = instance->clone();
			data->title = title;
			data->loadJson(root);
			spMod.reset(data);
		}
		else
			std::cout << "\n" << "Can't Deduce Type [" << ClassName << "]\n" << FILELINE;

		return spMod;
	}
	template <typename T>
	sptr<const T> getData(const String& rBPName, const std::map<String, sptr<const T> >& blueprints) const
	{
		auto it = blueprints.find(rBPName);

		if(it != blueprints.end())
			return it->second;
		else
		{
			std::cout << "\nCouldnt find [" << rBPName << "]." << FILELINE;
			return blueprints.begin()->second;
		}
	}

	std::map<String, sptr<const ModuleData> > m_modBP;
	std::map<String, sptr<const WeaponData> > m_wepBP;
	std::map<String, sptr<const ChunkData> > m_cnkBP;
	std::map<String, sptr<const ProjectileData> > m_prjBP;
	std::map<String, sptr<const StaticDecorData> > m_sdcBP;
	std::map<String, sptr<const DynamicDecorData> > m_ddcBP;
	std::map<String, sptr<const Particles> > m_particlesBP;
};