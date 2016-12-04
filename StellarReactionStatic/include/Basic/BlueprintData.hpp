#pragma once

#include "stdafx.hpp"

/// The interface for blueprint data.
struct BlueprintData
{
	BlueprintData()
	{
		title = "DEFAULT_TITLE";
	}

	String title;

	virtual void inheritPreHook(const BlueprintData& parent)
	{
		const String titleTemp;
		inherit(parent);
		title = titleTemp;
	}

	/// Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root) = 0;
	/// Make a copy of this object.
	virtual BlueprintData* clone() const = 0;

protected:
	virtual void inherit(const BlueprintData& parent)
	{
		*this = parent;
	}
};

