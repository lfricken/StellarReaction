#include "Stealth.hpp"
#include "Controller.hpp"

using namespace std;

Stealth::Stealth(const StealthData& rData) : ShipModule(rData)
{
	m_eConsump = rData.energyConsumption;
}
Stealth::~Stealth()
{

}

void Stealth::postPhysUpdate()
{
	if (m_stealthTimer.isTimeUp())
	{
		m_parentChunk->getHull()->setAlpha(alpha_stealth_off);
		std::vector<sptr<Module>> moduleList = m_parentChunk->getModuleList();
		for (auto it = moduleList.begin(); it != moduleList.end(); ++it)
		{
			(*it)->toggleStealth(false);
		}
		out_of_energy = false;
	}
	else
	{
		float eThisStep = m_eConsump*game.getUniverse().getTimeStep();
		if (eThisStep <= m_pEnergyPool->getValue() && !out_of_energy)
		{
			m_pEnergyPool->changeValue(-eThisStep);
			if (m_pEnergyPool->getValue() < eThisStep)
				out_of_energy = true;
		}
		else
		{
			out_of_energy = true;
			m_parentChunk->getHull()->setAlpha(alpha_stealth_off);
			std::vector<sptr<Module>> moduleList = m_parentChunk->getModuleList();
			for (auto it = moduleList.begin(); it != moduleList.end(); ++it)
			{
				(*it)->toggleStealth(false);
			}
		}
	}

}

void Stealth::directive(map<Directive, bool>& rIssues)
{
	if (rIssues[Directive::Stealth])
	{
		m_stealthTimer.setCountDown(stealth_duration);
		if (m_stealthTimer.isTimeUp())
			m_stealthTimer.restartCountDown();
		m_parentChunk->getHull()->setAlpha(alpha_stealth_on);
		std::vector<sptr<Module>> moduleList = m_parentChunk->getModuleList();
		for (auto it = moduleList.begin(); it != moduleList.end(); ++it)
		{
			(*it)->toggleStealth(true);
		}
	}
}
