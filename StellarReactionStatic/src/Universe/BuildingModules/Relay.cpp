#include "Relay.hpp"


void deltaResources(Team thisTeam, bool opposing, Resources delta)
{
	if(opposing)
	{
		for(int i = (int)Team::MinTeam; i < (int)Team::MaxTeam; ++i)
		{
			if((Team)i == thisTeam)
			{
				Resources::ChangeResources(delta, (Team)i);
			}
		}
	}
	else
	{
		Resources::ChangeResources(delta, thisTeam);
	}
}
void RelayData::loadJson(const Json::Value& root)
{
	ShipModuleData::loadJson(root);

	GETJSON(resourceChange);
	GETJSON(opposingTeams);
}
Relay::Relay(const RelayData& rData) : ShipModule(rData)
{
	m_resourceChange = rData.resourceChange;
	m_opposingTeams = rData.opposingTeams;

	Team thisTeam = m_parent->getBodyComponent().getTeam();
	deltaResources(thisTeam, m_opposingTeams, m_resourceChange);
}
Relay::~Relay()
{
	Resources delta;
	delta.subtract(m_resourceChange);

	Team thisTeam = m_parent->getBodyComponent().getTeam();
	deltaResources(thisTeam, m_opposingTeams, delta);
}
