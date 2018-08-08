#include "Module.hpp"
#include "Controller.hpp"
#include "CommandInfo.hpp"
#include "Weapon.hpp"



void ModuleData::loadJson(const Json::Value& root)
{
	BlueprintableData::loadJson(root);

	LOADJSON(rangeModifiers);
	LOADJSON(requirements);

	GETJSON(name);
	GETJSON(collisionDamage);

	LOADJSON(ioComp);
	LOADJSON(fixComp);
	LOADJSON(nwComp);
}
Module::Module(const ModuleData& rData) : Blueprintable(rData), m_io(rData.ioComp, &Module::input, this), m_fix(rData.fixComp), rangeModifiers(rData.rangeModifiers)
{
	m_collisionDamage = rData.collisionDamage;
	m_parent = rData.parent;
	m_name = rData.name;

	m_fix.setIOPos(m_io.getPosition());
	m_fix.bindStartCB(&Module::startContactCB, this);
	m_fix.bindEndCB(&Module::endContactCB, this);

	rangeModifiers.tryApply();
	ranges = rData.rangeModifiers.ranges;
}
Module::~Module()
{

}

float Module::functionalCapacity()
{
	return 1.00f;
}
void Module::applyModifierRates()
{            
	rangeModifiers.applyRateModifiers(getGame()->getUniverse().getTimeStep());
}
void Module::prePhysUpdate()
{
	applyModifierRates();
}
void Module::postPhysUpdate()
{

}
void Module::directive(const CommandInfo& commands)
{

}
void Module::setAim(const Vec2& rTarget)
{

}
void Module::startContactCB(FixtureComponent* pOther)
{
	//HitEffect e;
	//e.effect = "";
	//e.normal = pOther->getCenter().to(this->m_fix.getCenter());
	//e.pos
	Weapon::damage(&getGame()->getUniverse().getUniverseIO(), pOther->getIOPos(), m_collisionDamage, m_io.getPosition(), m_parent->getBodyComponent().getTeam(), Vec2(0, 0), Vec2(0, 0), "");
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
Vec2 Module::getOffset() const
{
	return m_fix.getOffset();
}
const String& Module::getName() const
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

void Module::healToMax()
{

}
void Module::applyModifiers(bool apply)
{
	if(apply)
		rangeModifiers.tryApply();
	else
		rangeModifiers.tryRemove();

}
void Module::input(String rCommand, sf::Packet rData)
{
	if(rCommand == "damage")//dont spam console!
	{

	}
	else
	{
		assert(Print << "\nCommand [" << rCommand << "] not found in [" << m_io.getName() << "]." << FILELINE);
	}
}
