#pragma once

#include "stdafx.hpp"
#include "Team.hpp"
#include "CollisionCategory.hpp"
#include "Resources.hpp"

#define GETJSON(VARNAME) \
	 VARNAME = JSON::get(root, #VARNAME, VARNAME);

#define LOADJSON(BASEVAR) \
	 if(!root[#BASEVAR].isNull()) BASEVAR.loadJson(root[#BASEVAR]);


/// Provides more convenient loading of Json data. Use the define: GETJSON(varname)
class JSON
{
public:
	///Get a Vec2 from JSON array.
	static Vec2 get(const Json::Value& root, const String& fieldName, const Vec2& defaultValue);
	///Get a color from JSON.
	static sf::Color get(const Json::Value& root, const String& fieldName, const sf::Color& defaultValue);
	///Get a sf::Vector2f from a JSON array.
	static sf::Vector2f get(const Json::Value& root, const String& fieldName, const sf::Vector2f& defaultValue);
	///Get a String from JSON.
	static String get(const Json::Value& root, const String& fieldName, const String& defaultValue);
	///Get a String from JSON.
	static String get(const Json::Value& root, const String& fieldName, const char* pString);
	///Get a float from JSON.
	static float get(const Json::Value& root, const String& fieldName, float defaultValue);
	///Get a int from JSON.
	static int get(const Json::Value& root, const String& fieldName, int defaultValue);
	///Get a boolean from JSON.
	static bool get(const Json::Value& root, const String& fieldName, bool defaultValue);
	///Get a Team from JSON.
	static Team get(const Json::Value& root, const String& fieldName, Team defaultValue);
	///Get a Team from JSON.
	static Category get(const Json::Value& root, const String& fieldName, Category defaultValue);
	///Get a Team from JSON.
	static Mask get(const Json::Value& root, const String& fieldName, Mask defaultValue);
	/// <summary>
	/// Load resource value from JSON.
	/// </summary>
	static Resources get(const Json::Value& root, const String& fieldName, Resources defaultValue);

	//template<typename T>
	static List<String> get(const Json::Value& root, const String& fieldName, List<String> defaultValue)
	{
		if(root[fieldName].isNull())
			return defaultValue;

		const Json::Value jsonList = root[fieldName];
		List<String> list;
		for(auto it = jsonList.begin(); it != jsonList.end(); ++it)
		{
			list.push_back(get<String>(it));//get<T>(it)
		}
		return list;
	}

private:
	//template <typename T>
	//T get() { return 0; };
	//template < >
	//float get<float>() { return 1.5; };
	//template < >
	//int get<int>() { return 2; };
	//template < >
	//double get<double>() { return 2.666666666; };
	//template < >
	//String get<String>() { return "lol"; };

	template <typename T>
	static T get(const Json::ValueConstIterator& itRef)
	{
		return itRef->asString();
	}
	template < >
	static String get<String>(const Json::ValueConstIterator& itRef)
	{
		return itRef->asString();
	}
	template < >
	static int get<int>(const Json::ValueConstIterator& itRef)
	{
		return itRef->asInt();
	}
	template < >
	static float get<float>(const Json::ValueConstIterator& itRef)
	{
		return itRef->asFloat();
	}
	template < >
	static double get<double>(const Json::ValueConstIterator& itRef)
	{
		return itRef->asDouble();
	}
	template < >
	static bool get<bool>(const Json::ValueConstIterator& itRef)
	{
		return itRef->asBool();
	}
};


