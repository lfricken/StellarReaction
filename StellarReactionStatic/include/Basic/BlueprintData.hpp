#pragma once

#include "stdafx.hpp"

//The interface for blueprint data.
struct BlueprintData
{
	BlueprintData()
	{
		title = "DEFAULT_TITLE";
	}

	std::string title;

	//Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root) = 0;
};

