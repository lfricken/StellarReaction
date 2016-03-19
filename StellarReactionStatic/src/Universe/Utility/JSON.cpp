#include "JSON.hpp"

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

