#include "Stealth.hpp"
#include "Controller.hpp"

using namespace std;

Stealth::Stealth(const StealthData& rData) : ShipModule(rData)
{

}
Stealth::~Stealth()
{

}

void Stealth::postPhysUpdate()
{
	if (m_stealthTimer.isTimeUp())
	{
		m_parentChunk->getHull()->setAlpha(255);
		std::vector<sptr<Module>> moduleList = m_parentChunk->getModuleList();
		for (auto it = moduleList.begin(); it != moduleList.end(); ++it)
		{
			(*it)->stealthOn(false);
		}
	}

}

void Stealth::directive(map<Directive, bool>& rIssues)
{
	if (rIssues[Directive::Stealth])
	{
		m_stealthTimer.setCountDown(5.0);
		if (m_stealthTimer.isTimeUp())
			m_stealthTimer.restartCountDown();
		m_parentChunk->getHull()->setAlpha(50);
		std::vector<sptr<Module>> moduleList = m_parentChunk->getModuleList();
		for (auto it = moduleList.begin(); it != moduleList.end(); ++it)
		{
			(*it)->stealthOn(true);
		}
	}
}
