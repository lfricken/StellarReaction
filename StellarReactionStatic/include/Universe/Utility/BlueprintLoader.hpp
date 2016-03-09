#pragma once

#include "stdafx.hpp"

struct ModuleData;
struct ProjectileData;
struct ChunkData;
struct WeaponData;

class BlueprintLoader
{
public:
	BlueprintLoader();
	virtual ~BlueprintLoader();

	void storeRoster(const std::string& rDir);

	sptr<const ModuleData> getModuleSPtr(const std::string& rBPName) const;
	sptr<const ChunkData> getChunkSPtr(const std::string& rBPName) const;
	sptr<const WeaponData> getWeaponSPtr(const std::string& rBPName) const;
	sptr<const ProjectileData> getProjectileSPtr(const std::string& rBPName) const;

	void storeModule(const std::string& rFile);
	void storeChunk(const std::string& rFile);
	void storeWeapon(const std::string& rFile);
	void storeProjectile(const std::string& rFile);

	sptr<ChunkData> loadChunk(const Json::Value& root);
	sptr<const ProjectileData> loadProjectile(const Json::Value& root);
	sptr<const WeaponData> loadWeapon(const Json::Value& root);
	sptr<const ModuleData> loadModule(const Json::Value& root);

	static sf::Color loadColor(const Json::Value& root);
protected:
private:

	std::map<std::string, sptr<const ModuleData> > m_modBP;
	std::map<std::string, sptr<const WeaponData> > m_wepBP;
	std::map<std::string, sptr<const ChunkData> > m_cnkBP;
	std::map<std::string, sptr<const ProjectileData> > m_prjBP;
};
