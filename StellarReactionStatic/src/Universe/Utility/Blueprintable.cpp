#include "Blueprintable.hpp"
#include "JSON.hpp"
#include "UpgradeType.hpp"


void BlueprintableData::setTeam(Team team)
{
	title = title + String((int)team);
}
void BlueprintableData::applyUpgrade(UpgradeType type)
{
	upgradeLevel++;
}
void BlueprintableData::loadJson(const Json::Value& root)
{
	GETJSON(title);
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

