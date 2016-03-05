#include "ShipModule.hpp"
#include "SoundManager.hpp"

using namespace std;

ShipModule::ShipModule(const ShipModuleData& rData) : Module(rData), m_health(rData.health)
{
	m_decors.push_back(sptr<GraphicsComponent>(new QuadComponent(rData.baseDecor)));
	m_baseDecor = m_decors.size() - 1;

	m_healthState = rData.initHealthState;
	m_functionsDamaged = rData.functionsDamaged;

	/**EXPLOSIONS**/
	QuadComponentData data;


	data.dimensions.x = 256;
	data.dimensions.y = 256;
	data.permanentRot = 0;
	data.center.x = 0;
	data.center.y = 0;
	data.texName = "effects/module_hit.png";
	data.animSheetName = "effects/module_hit.acfg";
	data.layer = GraphicsLayer::ShipAppendagesLower;
	m_decors.push_back(sptr<GraphicsComponent>(new QuadComponent(data)));
	m_hitDecorIndex = m_decors.size() - 1;


	data.dimensions.x = 256;
	data.dimensions.y = 256;
	data.permanentRot = 0;
	data.center.x = 0;
	data.center.y = 0;
	data.texName = "effects/explosion1.png";
	data.animSheetName = "effects/explosion1.acfg";
	data.layer = GraphicsLayer::ShipAppendagesLower;
	m_decors.push_back(sptr<GraphicsComponent>(new QuadComponent(data)));
	m_explosionIndex = m_decors.size() - 1;

	m_controlGroup = 1;
	//m_fix.setTeam();
}
ShipModule::~ShipModule()
{

}
void ShipModule::prePhysUpdate()
{

}
void ShipModule::postPhysUpdate()
{
	b2Vec2 center = m_fix.getCenter();
	float angle = m_fix.getAngle();
	for(int i = 0; i < (signed)m_decors.size(); ++i)
	{
		m_decors[i]->setPosition(center);
		m_decors[i]->setRotation(angle);
	}
}
void ShipModule::pack(sf::Packet& rPacket)
{

}
void ShipModule::unpack(sf::Packet& rPacket)
{

}
void ShipModule::input(std::string rCommand, sf::Packet rData)
{
	const float damagedPercent = 0.25;

	if(rCommand == "damage")
	{
		int val;
		rData >> val;


		m_health.damage(val);
		m_io.event(EventType::Health, m_health.getHealth(), voidPacket);

		m_decors[m_hitDecorIndex]->getAnimator().setAnimation("Hit", 0.20f);
		m_decors[m_hitDecorIndex]->setColor(sf::Color(255, 255 * m_health.getHealthPercent(), 0, 255));

		if(m_health.isDead())
		{
			setHealthState(HealthState::Broken);
		}
		else if(m_health.getHealthPercent() < damagedPercent)
		{
			setHealthState(HealthState::Damaged);
		}
	}
	else if(rCommand == "heal")
	{
		int val;
		rData >> val;


		m_health.heal(val);
		m_io.event(EventType::Health, m_health.getHealth(), voidPacket);
		if(m_health.getHealthPercent() >= damagedPercent)
			setHealthState(HealthState::Nominal);
	}
	else
		Module::input(rCommand, rData);
}
bool ShipModule::functioning()//does this module still do its function
{
	if(m_healthState == HealthState::Nominal)
		return true;
	else if(m_healthState == HealthState::Broken)
		return false;
	else if(m_healthState == HealthState::Damaged)
		return m_functionsDamaged;

	cout << "\n" << FILELINE;
	return true;
}
void ShipModule::setHealthState(HealthState newState)
{
	m_healthState = newState;

	if(m_healthState == HealthState::Nominal)
	{
		m_fix.setCategory(Category::ShipModule);
		m_decors[m_baseDecor]->setColor(sf::Color(255, 255, 255, 255));///makes the sprite red when destroyed
	}
	else if(m_healthState == HealthState::Broken)
	{
		m_fix.setCategory(Category::ShipModuleBroke);
		m_decors[m_baseDecor]->setColor(sf::Color(255, 0, 0, 255));///makes the sprite red when destroyed
		f_died();
	}
	setHealthStateHook(m_healthState);
}
void ShipModule::setHealthStateHook(HealthState newState)
{

}

int ShipModule::getControlGroup() const{
	return m_controlGroup;
}

void ShipModule::setControlGroup(int control_group) {
	m_controlGroup = control_group;
}

void ShipModule::f_died()
{
	b2Vec2 center = m_fix.getCenter();
	m_decors[m_hitDecorIndex]->getAnimator().setAnimation("Default", 1.0f);

	m_decors[m_explosionIndex]->getAnimator().setAnimation("Explode", 1.0f);
	m_decors[m_explosionIndex]->setPosition(center);

	SoundData sound;
	sound.name = "explode1.wav";
	sound.pos = center;
	game.getSound().playSound(sound);
}

void ShipModule::stealthOn(bool toggle)
{
	if (toggle)
		m_decors[m_baseDecor]->setAlpha(50);
	else
		m_decors[m_baseDecor]->setAlpha(255);
}

void ShipModule::healToMax()
{
	m_health.heal(m_health.getMaxHealth());
	setHealthState(HealthState::Nominal);
}
