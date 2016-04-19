#include "JSON.hpp"

b2Vec2 JSON::get(const Json::Value& root, const std::string& fieldName, const b2Vec2& defaultValue)
{
	if(root[fieldName].isNull())
		return defaultValue;
	else
		return b2Vec2(root[fieldName][0].asFloat(), root[fieldName][1].asFloat());
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
		return sf::Color(root[fieldName][0].asFloat(), root[fieldName][1].asFloat(), root[fieldName][2].asFloat(), root[fieldName][3].asFloat());
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
	std::string defaultValue(pString);//string is double overloaded because string literals decay to pointers, which decay to bools

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

