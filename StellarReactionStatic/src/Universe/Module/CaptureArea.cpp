#include "CaptureArea.hpp"
#include "BodyComponent.hpp"
#include "FixtureComponent.hpp"



void CaptureAreaData::loadJson(const Json::Value& root)
{
	GETJSON(value);
	GETJSON(captureTime);
	GETJSON(capturePercent);

	SensorData::loadJson(root);
}
CaptureArea::CaptureArea(const CaptureAreaData& rData) : Sensor(rData)
{
	m_progress = 0;
	m_oldProgress = 0;
	m_owned = false;
	m_currentTeam = Team::Invalid;
	m_capTeam = Team::Invalid;
	m_conflicted = false;

	m_value = rData.value;
	m_capPercent = rData.capturePercent;
	m_captureTime = rData.captureTime;
}
CaptureArea::~CaptureArea()
{

}
bool CaptureArea::isConflicted()
{
	return m_conflicted;
}
Team CaptureArea::getCurrentTeam()
{
	return m_currentTeam;
}
float CaptureArea::getProgress()
{
	return m_progress / m_captureTime;
}
void CaptureArea::prePhysUpdate()
{
	float time = m_capTimer.getTimeElapsed();
	m_conflicted = false;
	m_capTeam = Team::Invalid;

	Team firstGuestTeam = Team::Invalid;
	for(auto it = m_guests.cbegin(); it != m_guests.cend(); ++it)
	{
		Team thisTeam = (*it)->getTeam();

		if(thisTeam != Team::Neutral)//ignore asteroids
		{
			if(firstGuestTeam == Team::Invalid)//record the first team in here.
			{
				firstGuestTeam = thisTeam;
				m_capTeam = thisTeam;
			}
			if(firstGuestTeam != thisTeam)//if any team in here is not equal to the first, then it is conflicted.
			{
				m_conflicted = true;
				m_capTeam = Team::Invalid;
				break;
			}
		}
	}

	//progress the state to captured
	if(m_capTeam != Team::Invalid)
	{
		if(m_capTeam == m_currentTeam)//if capture team == owning team, add capture value
		{
			m_progress += time;
			if(m_progress >= m_captureTime)
				m_progress = m_captureTime;
		}
		else//otherwise lower capture value
		{
			m_progress -= time;
			if(m_progress <= 0)
				m_currentTeam = m_capTeam;
		}
	}

	//we transitioned to owning it
	if((m_oldProgress / m_captureTime) < m_capPercent && (m_progress / m_captureTime) >= m_capPercent)
	{
		m_owned = true;
		//getGame()->getUniverse().changeTeamMoney(m_currentTeam, m_value);
	}
	//we transitioned to not owning it
	else if((m_oldProgress / m_captureTime) > m_capPercent && (m_progress / m_captureTime) <= m_capPercent)
	{
		m_owned = false;
		//getGame()->getUniverse().changeTeamMoney(m_currentTeam, -m_value);
	}

	m_oldProgress = m_progress;
}

