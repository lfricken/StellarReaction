#include "ControlFactory.hpp"
#include "Controller.hpp"

using namespace std;

ControlFactory::ControlFactory()
{
	m_spNWFactory.reset(new NetworkFactory("control"));//this has to happen before we create any controllers!
}
ControlFactory::~ControlFactory()
{

}
void ControlFactory::processAllDirectives()
{
	for(auto it = m_spControlList.begin(); it != m_spControlList.end(); ++it)
		(*it)->processDirectives();
}
void ControlFactory::addController(const std::string& slave)
{
	ControllerData data;
	data.slaveName = slave;
	m_spControlList.push_back(sptr<Controller>(new Controller(data)));
}
void ControlFactory::resetControllers(const std::vector<std::string>& slaves)
{
	m_spControlList.clear();
	for(auto it = slaves.begin(); it != slaves.end(); ++it)
		addController(*it);
}
void ControlFactory::unsetLocal()
{
	for(auto it = m_spControlList.begin(); it != m_spControlList.end(); ++it)
		(*it)->toggleLocal(false);
}
Controller& ControlFactory::getController(int index)
{
	if((signed)m_spControlList.size() > index && index >= 0)
		return *m_spControlList[index];
	else
	{
		if(!m_spBackupController)
		{
			m_spBackupController.reset(new Controller(ControllerData()));
		}
		return *m_spBackupController;
	}
}
NetworkFactory& ControlFactory::getNWFactory()
{
	return *m_spNWFactory;
}
