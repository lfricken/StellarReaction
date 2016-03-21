#pragma once

#include "stdafx.hpp"
#include "Globals.hpp"

struct ModuleData;
struct ProjectileData;
struct ChunkData;
struct WeaponData;

class BlueprintLoader
{
public:
	BlueprintLoader();
	virtual ~BlueprintLoader();

	void loadBlueprints(const std::string& rDir);

	sptr<const ModuleData> getModuleSPtr(const std::string& rBPName) const;
	sptr<const ChunkData> getChunkSPtr(const std::string& rBPName) const;
	sptr<const WeaponData> getWeaponSPtr(const std::string& rBPName) const;
	sptr<const ProjectileData> getProjectileSPtr(const std::string& rBPName) const;

	static sf::Color loadColor(const Json::Value& root);
protected:
private:

	/** IMPORTANT NOTE:
	/** Changing this file doesn't always trigger a recompile, you may need to manually do it!
	**/
	template <typename T>
	void storeData(const std::string& rFile, std::map<std::string, sptr<const T> >& blueprints)//load that blueprint
	{
		std::ifstream stream(rFile, std::ifstream::binary);
		Json::Reader reader;
		Json::Value root;
		bool parsedSuccess = reader.parse(stream, root, false);

		if(parsedSuccess)
			blueprints[root["title"].asString()] = loadData<T>(root);
		else
			std::cout << "\n" << FILELINE;
	}
	template <typename T>
	sptr<const T> loadData(const Json::Value& root)
	{
		sptr<const T> spMod;
		const std::string title = root["ClassName"].asString();
		const T* instance = Deduce<T>::from(title);

		if(instance != NULL)//we derived it!
		{
			T* data = instance->clone();
			data->loadJson(root);
			spMod.reset(data);
		}
		else
			std::cout << "\n" << "Couldn't Find [" << title << "]" << FILELINE;

		return spMod;
	}
	template <typename T>
	sptr<const T> getData(const std::string& rBPName, const std::map<std::string, sptr<const T> >& blueprints) const
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

	std::map<std::string, sptr<const ModuleData> > m_modBP;
	std::map<std::string, sptr<const WeaponData> > m_wepBP;
	std::map<std::string, sptr<const ChunkData> > m_cnkBP;
	std::map<std::string, sptr<const ProjectileData> > m_prjBP;
};
