#include "Extractor.hpp"


void changeIncome(Team team, Resources delta)
{
	Message changeIncome;
	sf::Packet data;
	data << (int)team;
	delta.intoPacket(&data);
	changeIncome.reset("universe", "changeTeamIncome", data, 0.f, false);

	Message::SendUniverse(changeIncome);
}

void deltaIncome(Team thisTeam, bool opposing, Resources delta)
{
	if(opposing)
	{
		for(int i = (int)Team::MinTeam; i < (int)Team::MaxTeam; ++i)
		{
			if((int)thisTeam != i)
			{
				changeIncome((Team)i, delta);
			}
		}
	}
	else
	{
		changeIncome(thisTeam, delta);
	}
}


void ExtractorData::loadJson(const Json::Value& root)
{
	ShipModuleData::loadJson(root);

	GETJSON(opposingTeams);
	GETJSON(incomeChange);
}
Extractor::Extractor(const ExtractorData& rData) : ShipModule(rData)
{
	m_incomeChange = rData.incomeChange;
	m_opposingTeams = rData.opposingTeams;

	Team thisTeam = m_parent->getBodyComponent().getTeam();
	deltaIncome(thisTeam, m_opposingTeams, m_incomeChange);
}
Extractor::~Extractor()
{
	Resources delta;
	delta.subtract(m_incomeChange);

	Team thisTeam = m_parent->getBodyComponent().getTeam();
	deltaIncome(thisTeam, m_opposingTeams, delta);
}


