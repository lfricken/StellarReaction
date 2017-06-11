#include "ControlFactory.hpp"
#include "Controller.hpp"

ControlFactory::ControlFactory()
{
	m_spNWFactory.reset(new NetworkFactory("control"));//this has to happen before we create any controllers!
}
ControlFactory::~ControlFactory()
{
	m_list.clear();
}
void ControlFactory::processAllDirectives()
{
	for(auto it = m_list.begin(); it != m_list.end(); ++it)
		(*it)->processDirectives();
}
int ControlFactory::addController(const String& slave)
{
	ControllerData data;
	data.slaveName = slave;
	m_list.push_back(sptr<Controller>(new Controller(data)));
	return m_list.size() - 1;
}
void ControlFactory::resetControllers(const List<String>& slaves)
{
	m_list.clear();
	for(auto it = slaves.begin(); it != slaves.end(); ++it)
		addController(*it);
}
void ControlFactory::setAllNonLocallyControlled()
{
	for(auto it = m_list.begin(); it != m_list.end(); ++it)
		(*it)->toggleLocal(false);
}
Controller* ControlFactory::getController(int index)
{
	if((signed)m_list.size() > index && index >= 0)
		return m_list[index].get();
	else
	{
		return NULL;
	}
}
int ControlFactory::getSize()
{
	return m_list.size();
}
NetworkFactory& ControlFactory::getNWFactory()//because base object clears
{
	return *m_spNWFactory;
}
