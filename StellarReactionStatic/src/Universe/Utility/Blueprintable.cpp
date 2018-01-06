#include "Blueprintable.hpp"
#include "JSON.hpp"
#include "UpgradeType.hpp"


void BlueprintableData::loadJson(const Json::Value& root)
{
	GETJSON(title);
}
void BlueprintableData::applyUpgrade(UpgradeType type)
{

}
Blueprintable::Blueprintable(const BlueprintableData& data)
{
	m_title = data.title;
}
Blueprintable::~Blueprintable()
{

}
const String& Blueprintable::getTitle() const
{
	return m_title;
}

