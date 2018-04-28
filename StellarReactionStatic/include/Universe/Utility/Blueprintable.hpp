#pragma once

#include "stdafx.hpp"
#include "Team.hpp"

struct BlueprintableData;
enum class UpgradeType;

class Blueprintable
{
public:
	Blueprintable(const BlueprintableData& data);
	virtual ~Blueprintable();

	const String& getTitle() const;

private:
	String m_title;
};


class ILoadsJson
{
public:
	/// <summary>
	/// Fill this object with data from a json file.
	/// </summary>
	virtual void loadJson(const Json::Value& root) = 0;
};

/// The interface for blueprint data.
struct BlueprintableData : public ILoadsJson
{
	BlueprintableData()
	{
		title = "VOIDCRAP";
		upgradeLevel = 0;
	}

	/// <summary>
	/// Sets this blueprint to be on a specific team.
	/// </summary>
	virtual void setTeam(Team team);

	/// <summary>
	/// Do NOT load from JSON. This is set by the blueprint loader class.
	/// </summary>
	String title;
	
	virtual void inheritPreHook(const BlueprintableData& parent)
	{
		inherit(parent);
	}

	/// <summary>
	/// Starts at 0. Represents how many times this module has been upgraded.
	/// </summary>
	int upgradeLevel;

	/// <summary>
	/// Fill this object with data from a json file.
	/// </summary>
	virtual void loadJson(const Json::Value& root);
	/// Make a copy of this object.
	virtual BlueprintableData* clone() const = 0;

	virtual void applyUpgrade(UpgradeType type);
protected:

	virtual void inherit(const BlueprintableData& parent)
	{
		*this = parent;
	}
};

