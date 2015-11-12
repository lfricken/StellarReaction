#include "CaptureArea.hpp"

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
		int thisTeam = (*it)->getTeam();
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
	}
	//we transitioned to not owning it
	else if((m_oldProgress / m_captureTime) > m_capPercent && (m_progress / m_captureTime) <= m_capPercent)
	{
		m_owned = false;
	}

	m_oldProgress = m_progress;
}
