#include "Module.hpp"
#include "Controller.hpp"
#include "CommandInfo.hpp"
#include "Weapon.hpp"

using namespace std;

void ModuleData::loadJson(const Json::Value& root)
{
	GETJSON(name);
	LOADJSON(ioComp);
	LOADJSON(fixComp);
	LOADJSON(nwComp);
	GETJSON(cost);
	GETJSON(collisionDamage);
}
Module::Module(const ModuleData& rData) : m_io(rData.ioComp, &Module::input, this), m_fix(rData.fixComp)
{
	m_collisionDamage = rData.collisionDamage;
	m_parentChunk = rData.chunkParent;
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
void Module::directive(const CommandInfo& commands)
{

}
void Module::setAim(const b2Vec2& rTarget)
{

}
void Module::startContactCB(FixtureComponent* pOther)
{
	Weapon::damage(&game.getUniverse().getUniverseIO(), pOther->getIOPos(), m_collisionDamage, m_io.getPosition());
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
const FixtureComponent& Module::getFixtureComponent()
{
	return m_fix;
}
void Module::toggleStealth(bool toggle)
{

}

void Module::healToMax(){

}
void Module::input(std::string rCommand, sf::Packet rData)
{
	if(rCommand == "setStore")
	{
		std::string name;
		rData >> name;
		m_fix.setStore(name);
	}
	if(rCommand == "damage")//dont spam console!
	{
	
	}
	else
		cout << "\nCommand [" << rCommand << "] not found in [" << m_io.getName() << "]." << FILELINE;
}

