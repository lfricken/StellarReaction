#include "JSON.hpp"

Vec2 JSON::get(const Json::Value& root, const std::string& fieldName, const Vec2& defaultValue)
{
	if(root[fieldName].isNull())
		return defaultValue;
	else
		return Vec2(root[fieldName][0].asFloat(), root[fieldName][1].asFloat());
}
sf::Vector2f JSON::get(const Json::Value& root, const std::string& fieldName, const sf::Vector2f& defaultValue)
{
	if(root[fieldName].isNull())
		return defaultValue;
	else
		return sf::Vector2f(root[fieldName][0].asFloat(), root[fieldName][1].asFloat());
}
sf::Color JSON::get(const Json::Value& root, const std::string& fieldName, const sf::Color& defaultValue)
{
	if(root[fieldName].isNull())
		return defaultValue;
	else
		return sf::Color(root[fieldName][0].asInt(), root[fieldName][1].asInt(), root[fieldName][2].asInt(), root[fieldName][3].asInt());
}








std::string JSON::get(const Json::Value& root, const std::string& fieldName, const std::string& defaultValue)
{
	if(root[fieldName].isNull())
		return defaultValue;
	else
		return root[fieldName].asString();
}
std::string JSON::get(const Json::Value& root, const std::string& fieldName, const char* pString)
{
	std::string defaultValue(pString);//string is double overloaded because string literals decay to pointers, which decay to bools and then call the JSON::get(bool stuff)

	if(root[fieldName].isNull())
		return defaultValue;
	else
		return root[fieldName].asString();
}
float JSON::get(const Json::Value& root, const std::string& fieldName, float defaultValue)
{
	if(root[fieldName].isNull())
		return defaultValue;
	else
		return root[fieldName].asFloat();
}
int JSON::get(const Json::Value& root, const std::string& fieldName, int defaultValue)
{
	if(root[fieldName].isNull())
		return defaultValue;
	else
		return root[fieldName].asInt();
}
bool JSON::get(const Json::Value& root, const std::string& fieldName, bool defaultValue)
{
	if(root[fieldName].isNull())
		return defaultValue;
	else
		return root[fieldName].asBool();
}
Team JSON::get(const Json::Value& root, const std::string& fieldName, Team defaultValue)
{
	if(root[fieldName].isNull())
		return defaultValue;
	else
		return (Team)root[fieldName].asInt();
}
