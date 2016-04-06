#pragma once

#include "stdafx.hpp"

struct BlueprintData
{
	BlueprintData()
	{
		title = "DEFAULT_TITLE";
	}

	std::string title;

	virtual void loadJson(const Json::Value& root) = 0;
};

