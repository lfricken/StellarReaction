#pragma once

#include "stdafx.hpp"

#define GETJSON(VARNAME) \
	 VARNAME = JSON::get(root, #VARNAME, VARNAME);

class JSON
{
public:
	static std::string get(const Json::Value& root, const std::string& fieldName, const std::string& defaultValue);
	static std::string get(const Json::Value& root, const std::string& fieldName, const char* pString);
	static float get(const Json::Value& root, const std::string& fieldName, float defaultValue);
	static int get(const Json::Value& root, const std::string& fieldName, int defaultValue);
	static bool get(const Json::Value& root, const std::string& fieldName, bool defaultValue);
};

