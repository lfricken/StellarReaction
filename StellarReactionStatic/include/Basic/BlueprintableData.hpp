#pragma once

#include "stdafx.hpp"

struct BlueprintableData;

class Blueprintable
{
public:
	Blueprintable(const BlueprintableData& data);
	virtual ~Blueprintable();

protected:
	String m_title;
};


/// The interface for blueprint data.
struct BlueprintableData
{
	BlueprintableData()
	{

	}

	/// <summary>
	/// Do NOT load from JSON. This is set by the blueprint loader class.
	/// </summary>
	String title;
	
	virtual void inheritPreHook(const BlueprintableData& parent)
	{
		inherit(parent);
	}

	/// Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root) = 0;
	/// Make a copy of this object.
	virtual BlueprintableData* clone() const = 0;

protected:

	virtual void inherit(const BlueprintableData& parent)
	{
		*this = parent;
	}
};

