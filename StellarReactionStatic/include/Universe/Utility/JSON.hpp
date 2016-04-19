#pragma once

#include "stdafx.hpp"

#define GETJSON(VARNAME) \
	 VARNAME = JSON::get(root, #VARNAME, VARNAME);

#define LOADJSON(BASEVAR) \
	 if(!root[#BASEVAR].isNull()) BASEVAR.loadJson(root[#BASEVAR]);

#define LOADJSONT(BASEVAR, TYPE) \
	 if(!root[#BASEVAR].isNull()) BASEVAR.loadJson<TYPE>(root[#BASEVAR]);

class JSON
{
public:
	static b2Vec2 get(const Json::Value& root, const std::string& fieldName, const b2Vec2& defaultValue);
	static sf::Color get(const Json::Value& root, const std::string& fieldName, const sf::Color& defaultValue);
	static sf::Vector2f get(const Json::Value& root, const std::string& fieldName, const sf::Vector2f& defaultValue);

	static std::string get(const Json::Value& root, const std::string& fieldName, const std::string& defaultValue);
	static std::string get(const Json::Value& root, const std::string& fieldName, const char* pString);
	static float get(const Json::Value& root, const std::string& fieldName, float defaultValue);
	static int get(const Json::Value& root, const std::string& fieldName, int defaultValue);
	static bool get(const Json::Value& root, const std::string& fieldName, bool defaultValue);
};

