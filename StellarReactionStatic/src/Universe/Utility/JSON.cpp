#include "JSON.hpp"

Vec2 JSON::get(const Json::Value& root, const String& fieldName, const Vec2& defaultValue)
{
	if(root[fieldName].isNull())
		return defaultValue;

	return Vec2(root[fieldName][0].asFloat(), root[fieldName][1].asFloat());
}
sf::Vector2f JSON::get(const Json::Value& root, const String& fieldName, const sf::Vector2f& defaultValue)
{
	if(root[fieldName].isNull())
		return defaultValue;

	return sf::Vector2f(root[fieldName][0].asFloat(), root[fieldName][1].asFloat());
}
sf::Color JSON::get(const Json::Value& root, const String& fieldName, const sf::Color& defaultValue)
{
	if(root[fieldName].isNull())
		return defaultValue;

	return sf::Color(root[fieldName][0].asInt(), root[fieldName][1].asInt(), root[fieldName][2].asInt(), root[fieldName][3].asInt());
}








String JSON::get(const Json::Value& root, const String& fieldName, const String& defaultValue)
{
	if(root[fieldName].isNull())
		return defaultValue;

	return root[fieldName].asString();
}
String JSON::get(const Json::Value& root, const String& fieldName, const char* defaultValue)
{
	if(root[fieldName].isNull())
		return defaultValue;

	return root[fieldName].asString();
}
float JSON::get(const Json::Value& root, const String& fieldName, float defaultValue)
{
	if(root[fieldName].isNull())
		return defaultValue;

	return root[fieldName].asFloat();
}
int JSON::get(const Json::Value& root, const String& fieldName, int defaultValue)
{
	if(root[fieldName].isNull())
		return defaultValue;

	return root[fieldName].asInt();
}
bool JSON::get(const Json::Value& root, const String& fieldName, bool defaultValue)
{
	if(root[fieldName].isNull())
		return defaultValue;

	return root[fieldName].asBool();
}
Team JSON::get(const Json::Value& root, const String& fieldName, Team defaultValue)
{
	if(root[fieldName].isNull())
		return defaultValue;

	return (Team)root[fieldName].asInt();
}
Category JSON::get(const Json::Value& root, const String& fieldName, Category defaultValue)
{
	if(root[fieldName].isNull())
		return defaultValue;

	return ChooseCategory(root[fieldName].asString());
}
Mask JSON::get(const Json::Value& root, const String& fieldName, Mask defaultValue)
{
	if(root[fieldName].isNull())
		return defaultValue;

	return ChooseMask(root[fieldName].asString());
}
Resources JSON::get(const Json::Value& root, const String& fieldName, Resources defaultValue)
{
	if(root[fieldName].isNull())
		return defaultValue;

	Resources res;
	int i = 0;
	for(auto it = res.m_resourceValues.begin(); it != res.m_resourceValues.end(); ++it)
	{
		if(i >= root[fieldName].size())
			break;

		int resValue = root[fieldName][i].asInt();
		it->second = resValue;
		++i;
	}

	return res;
}
