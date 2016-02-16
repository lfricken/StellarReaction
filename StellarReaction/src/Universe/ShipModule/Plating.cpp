#include "Plating.hpp"
#include "Controller.hpp"

using namespace std;

Plating::Plating(const PlatingData& rData) : ShipModule(rData)
{

}
Plating::~Plating()
{

}

void Plating::directive(map<Directive, bool>& rIssues)
{
	if (rIssues[Directive::Stealth])
	{
		setAlpha(m_parentChunk->getHull());
		std::vector<sptr<Module>> moduleList = m_parentChunk->getModuleList();
		for (auto it = moduleList.begin(); it != moduleList.end(); ++it)
		{
			(*it)->stealthOn(true);
		}
	}
}
