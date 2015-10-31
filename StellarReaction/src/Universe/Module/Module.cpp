#include "Module.hpp"

#include "Controller.hpp"

using namespace std;

Module::Module(const ModuleData& rData) : m_io(rData.ioComp, &Module::input, this), m_nw(rData.nwComp, &Module::pack, &Module::unpack, this, game.getNwBoss().getNWFactory()), m_fix(rData.fixComp)
{
	m_title = rData.title;
	m_name = rData.name;

	m_fix.setIOPos(m_io.getPosition());
	m_fix.bindStartCB(&Module::startContactCB, this);
	m_fix.bindEndCB(&Module::endContactCB, this);

	m_pMissilePool = rData.pools.missilePool;
	m_pEnergyPool = rData.pools.energyPool;
	m_pZoomPool = rData.pools.zoomPool;
	m_pBallisticPool = rData.pools.ballisticPool;
}
Module::~Module()
{

}
void Module::postPhysUpdate()
{

}
void Module::directive(std::map<Directive, bool>& rIssues)
{

}
void Module::setAim(const b2Vec2& rTarget)
{

}
void Module::startContactCB(FixtureComponent* pOther)
{

}
void Module::endContactCB(FixtureComponent* pOther)
{

}
void Module::pack(sf::Packet& rPacket)
{

}
void Module::unpack(sf::Packet& rPacket)
{

}
const std::string& Module::getStore() const
{
	return m_fix.getStore();
}
const b2Vec2& Module::getOffset() const
{
	return m_fix.getOffset();
}
const std::string& Module::getTitle() const
{
	return m_title;
}
const std::string& Module::getName() const
{
	return m_name;
}
void Module::input(std::string rCommand, sf::Packet rData)
{
	if(rCommand == "setStore")
	{
		std::string name;
		rData >> name;
		m_fix.setStore(name);
	}
	else
		cout << "\nCommand not found in [" << m_io.getName() << "]." << FILELINE;
}
