#include "CaptureArea.hpp"
#include "BodyComponent.hpp"
#include "FixtureComponent.hpp"

Register(ModuleData, CaptureAreaData);
CaptureArea::CaptureArea(const CaptureAreaData& rData) : Sensor(rData)
{
	m_progress = 0;
	m_oldProgress = 0;
	m_owned = false;
	m_currentTeam = -1;

	m_value = rData.value;
	m_capPercent = rData.capturePercent;
	m_captureTime = rData.captureTime;
}
CaptureArea::~CaptureArea()
{

}
void CaptureArea::prePhysUpdate()
{
	int team = -1;
	bool conflicted = false;
	float time = m_capTimer.getTimeElapsed();

	for(auto it = m_guests.cbegin(); it != m_guests.cend(); ++it)
	{
		void* p = (*it)->getBodyPtr()->GetUserData();
		int thisTeam = static_cast<BodyComponent*>(p)->getTeam();
		if(team == -1 || team == thisTeam)
			team = thisTeam;
		else
		{
			conflicted = true;
			break;
		}
	}

	if(m_guests.size() > 0 && !conflicted)
	{
		if(team == m_currentTeam)
		{
			m_progress += time;
			if(m_progress >= m_captureTime)
				m_progress = m_captureTime;
		}
		else
		{
			m_progress -= time;
			if(m_progress <= 0)
				m_currentTeam = team;
		}
	}

	//we transitioned to owning it
	if((m_oldProgress / m_captureTime) < m_capPercent && (m_progress / m_captureTime) >= m_capPercent)
	{
		m_owned = true;
		game.getUniverse().changeTeamMoney(m_currentTeam, m_value);
	}
	//we transitioned to not owning it
	else if((m_oldProgress / m_captureTime) > m_capPercent && (m_progress / m_captureTime) <= m_capPercent)
	{
		m_owned = false;
		game.getUniverse().changeTeamMoney(m_currentTeam, -m_value);
	}

	m_oldProgress = m_progress;
}
void CaptureAreaData::loadJson(const Json::Value& root)
{
	if(!root["Value"].isNull())
		value = root["Value"].asInt();
	if(!root["CaptureTime"].isNull())
		captureTime = root["CaptureTime"].asInt();
	if(!root["CapturePercent"].isNull())
		capturePercent = root["CapturePercent"].asFloat();

	SensorData::loadJson(root);
}
