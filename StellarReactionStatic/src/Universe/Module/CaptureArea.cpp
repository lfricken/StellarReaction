#include "CaptureArea.hpp"
#include "BodyComponent.hpp"
#include "FixtureComponent.hpp"

using namespace std;

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
	m_currentTeam = -1;
	m_capTeam = -1;
	m_conflicted = false;

	m_value = rData.value;
	m_capPercent = rData.capturePercent;
	m_captureTime = rData.captureTime;
}
CaptureArea::~CaptureArea()
{

}
bool CaptureArea::isConflicted(){
	return m_conflicted;
}
int CaptureArea::getCurrentTeam(){
	return m_currentTeam;
}
float CaptureArea::getProgress(){
	return m_progress / m_captureTime;
}
void CaptureArea::prePhysUpdate()
{
	m_conflicted = false;
	float time = m_capTimer.getTimeElapsed();
	m_capTeam = -1;

	for (auto it = m_guests.cbegin(); it != m_guests.cend(); ++it)
	{
		void* p = (*it)->getBodyPtr()->GetUserData();
		int thisTeam = static_cast<BodyComponent*>(p)->getTeam();
		if (thisTeam != -784){//ignore asteroids
			if (m_capTeam == -1 || m_capTeam == thisTeam)
				m_capTeam = thisTeam;
			else
			{
				m_conflicted = true;
				m_capTeam = -1;
				break;
			}
		}
	}

	if (!m_conflicted && m_capTeam > 0)
	{
		if (m_capTeam == m_currentTeam)
		{
			m_progress += time;
			if (m_progress >= m_captureTime)
				m_progress = m_captureTime;
		}
		else
		{
			m_progress -= time;
			if (m_progress <= 0)
				m_currentTeam = m_capTeam;
		}
	}

	//we transitioned to owning it
	if ((m_oldProgress / m_captureTime) < m_capPercent && (m_progress / m_captureTime) >= m_capPercent)
	{
		m_owned = true;
		game.getUniverse().changeTeamMoney(m_currentTeam, m_value);
	}
	//we transitioned to not owning it
	else if ((m_oldProgress / m_captureTime) > m_capPercent && (m_progress / m_captureTime) <= m_capPercent)
	{
		m_owned = false;
		game.getUniverse().changeTeamMoney(m_currentTeam, -m_value);
	}

	m_oldProgress = m_progress;
	//cout << m_currentTeam;
}

