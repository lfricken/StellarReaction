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
		if(*it != nullptr)
			(*it)->processDirectives();
}
int ControlFactory::addController(const String& slave)
{
	ControllerData data;
	data.slaveName = slave;
	auto controller = sptr<Controller>(new Controller(data));
	m_list.insert(controller);
	return controller->getFactoryPosition();
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
	return m_list.get(index);
}
int ControlFactory::getSize()
{
	return m_list.size();
}
NetworkFactory& ControlFactory::getNWFactory()//because base object clears
{
	return *m_spNWFactory;
}
