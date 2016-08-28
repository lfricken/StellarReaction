#pragma once

#include "stdafx.hpp"
#include "Team.hpp"

#define GETJSON(VARNAME) \
	 VARNAME = JSON::get(root, #VARNAME, VARNAME);

#define LOADJSON(BASEVAR) \
	 if(!root[#BASEVAR].isNull()) BASEVAR.loadJson(root[#BASEVAR]);

#define LOADJSONT(BASEVAR, TYPE) \
	 if(!root[#BASEVAR].isNull()) BASEVAR.loadJson<TYPE>(root[#BASEVAR]);

/// Provides more convenient loading of Json data. Use the define: GETJSON(varname)
class JSON
{
public:
	///Get a Vec2 from JSON array.
	static Vec2 get(const Json::Value& root, const std::string& fieldName, const Vec2& defaultValue);
	///Get a color from JSON.
	static sf::Color get(const Json::Value& root, const std::string& fieldName, const sf::Color& defaultValue);
	///Get a sf::Vector2f from a JSON array.
	static sf::Vector2f get(const Json::Value& root, const std::string& fieldName, const sf::Vector2f& defaultValue);
	///Get a string from JSON.
	static std::string get(const Json::Value& root, const std::string& fieldName, const std::string& defaultValue);
	///Get a string from JSON.
	static std::string get(const Json::Value& root, const std::string& fieldName, const char* pString);
	///Get a float from JSON.
	static float get(const Json::Value& root, const std::string& fieldName, float defaultValue);
	///Get a int from JSON.
	static int get(const Json::Value& root, const std::string& fieldName, int defaultValue);
	///Get a boolean from JSON.
	static bool get(const Json::Value& root, const std::string& fieldName, bool defaultValue);
	///Get a boolean from JSON.
	static Team get(const Json::Value& root, const std::string& fieldName, Team defaultValue);
};

