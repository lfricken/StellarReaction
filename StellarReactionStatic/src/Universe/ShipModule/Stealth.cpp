#include "Stealth.hpp"
#include "Controller.hpp"




void StealthData::loadJson(const Json::Value& root)
{
	GETJSON(energyConsumption);

	ShipModuleData::loadJson(root);
}
Stealth::Stealth(const StealthData& rData) : ShipModule(rData)
{
	m_eConsump = rData.energyConsumption;
}
Stealth::~Stealth()
{

}
void Stealth::postPhysUpdate()
{
	//if(m_stealthTimer.isTimeUp() || !isFunctioning())
	//{
	//	m_parent->getHull()->setAlpha(alpha_stealth_off);
	//	List<sptr<Module>> moduleList = m_parentChunk->getModuleList();
	//	m_parentChunk->setStealth(false);
	//	for(auto it = moduleList.begin(); it != moduleList.end(); ++it)
	//	{
	//		(*it)->toggleStealth(false);
	//	}
	//	out_of_energy = false;
	//}
	//else
	//{
	//	auto& energyPool = (*ranges)[RangeList::Energy];
	//	float eThisStep = m_eConsump*getGame()->getUniverse().getTimeStep();

	//	if(energyPool.tryChange(-eThisStep) && !out_of_energy)
	//	{
	//		if(energyPool.getValue() < eThisStep)
	//			out_of_energy = true;
	//	}
	//	else
	//	{
	//		out_of_energy = true;
	//		m_parentChunk->getHull()->setAlpha(alpha_stealth_off);
	//		List<sptr<Module>> moduleList = m_parentChunk->getModuleList();
	//		m_parentChunk->setStealth(false);
	//		for(auto it = moduleList.begin(); it != moduleList.end(); ++it)
	//		{
	//			(*it)->toggleStealth(false);
	//		}
	//	}
	//}
	ShipModule::postPhysUpdate();
}
void Stealth::directive(const CommandInfo& commands)
{
	//Map<Directive, bool> rIssues = commands.directives;
	//if(rIssues[Directive::Stealth] && isFunctioning())
	//{
	//	m_stealthTimer.setCountDown(stealth_duration);
	//	if(m_stealthTimer.isTimeUp())
	//		m_stealthTimer.restartCountDown();
	//	m_parentChunk->getHull()->setAlpha(alpha_stealth_on);
	//	List<sptr<Module>> moduleList = m_parentChunk->getModuleList();
	//	m_parentChunk->setStealth(true);
	//	for(auto it = moduleList.begin(); it != moduleList.end(); ++it)
	//	{
	//		(*it)->toggleStealth(true);
	//	}
	//}
}



