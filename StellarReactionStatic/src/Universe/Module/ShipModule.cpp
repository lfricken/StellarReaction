#include "ShipModule.hpp"
#include "SoundManager.hpp"
#include "Player.hpp"
#include "Team.hpp"

using namespace std;

void ShipModuleData::loadJson(const Json::Value& root)
{
	LOADJSON(health);
	LOADJSON(baseDecor);
	LOADJSON(deathSound);

	ModuleData::loadJson(root);
}

ShipModule::ShipModule(const ShipModuleData& rData) : Module(rData), m_health(rData.health)
{
	m_deathSound = rData.deathSound;
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

	//m_fix.setTeam();//TODO WHAT IS THIS
}
ShipModule::~ShipModule()
{

}
const Health ShipModule::getHealth()
{
	return m_health;
}
void ShipModule::prePhysUpdate()
{

}
void ShipModule::postPhysUpdate()
{
	Vec2 center = m_fix.getCenter();
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
void ShipModule::input(String rCommand, sf::Packet rData)
{
	const float damagedPercent = 0.25;// TODO should be some global constant

	if(rCommand == "damage")//TODO huge if statement needs refactor
	{
		int val;
		int cause;
		Team damagingTeam;
		int teamTemp;
		rData >> val >> cause >> teamTemp;
		damagingTeam = static_cast<Team>(teamTemp);
		Team myTeam = m_parentChunk->getBodyComponent().getTeam();

		const bool damagePositive = val > 0;
		const bool differentTeams = damagingTeam != myTeam || damagingTeam == Team::Alone;
		const bool validTeams = (damagingTeam != Team::Invalid && damagingTeam != Team::Capturable) && (myTeam != Team::Invalid && myTeam != Team::Neutral && myTeam != Team::Capturable);

		if(damagePositive && differentTeams && validTeams)
		{
			m_health.damage(val);
			game.getUniverse().spawnParticles("LowSparks", m_fix.getCenter(), Vec2(-1, 0), Vec2(0, 0));
			m_io.event(EventType::Health, m_health.getHealth(), voidPacket);

			m_decors[m_hitDecorIndex]->getAnimator().setAnimation("Hit", 0.20f);
			m_decors[m_hitDecorIndex]->setColor(sf::Color(255, static_cast<char>(255.f * m_health.getHealthPercent()), 0, 255));

			if(m_healthState != HealthState::Broken)
			{
				if(m_health.isDead())
				{
					setHealthState(HealthState::Broken);
					sf::Packet pack;
					pack << 1;
					Message mess(cause, "increase_score", pack, 0.0f, false);
					game.getUniverse().getUniverseIO().recieve(mess);
				}
				else if(m_health.getHealthPercent() < damagedPercent)
				{
					setHealthState(HealthState::Damaged);
				}
			}
		}
	}
	else if(rCommand == "heal")
	{
		int val;
		int cause;
		rData >> val >> cause;

		m_health.heal(val);
		m_io.event(EventType::Health, m_health.getHealth(), voidPacket);
		if(m_health.getHealthPercent() >= damagedPercent)
			setHealthState(HealthState::Nominal);
	}
	else if(rCommand == "increase_score")
	{
		int val;
		rData >> val;
		this->m_parentChunk->increaseScore();
	}
	else
		Module::input(rCommand, rData);
}
bool ShipModule::isFunctioning()//does this module still do its function
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
void ShipModule::f_died()
{
	Vec2 center = m_fix.getCenter();
	m_decors[m_hitDecorIndex]->getAnimator().setAnimation("Default", 1.0f);

	m_decors[m_explosionIndex]->getAnimator().setAnimation("Explode", 1.0f);
	m_decors[m_explosionIndex]->setPosition(center);

	m_deathSound.play(center);

	//Sound needed.
	//SoundData sound;
	//sound.name = "ExplodeSmall.wav";
	//sound.pos = center;
	//game.getSound().playSound(sound);
}
void ShipModule::toggleStealth(bool toggle)
{
	if(toggle)
		m_decors[m_baseDecor]->setAlpha(alpha_stealth_on);
	else
		m_decors[m_baseDecor]->setAlpha(alpha_stealth_off);
}
void ShipModule::healToMax()
{
	sf::Packet packet;
	packet << m_health.getMaxHealth() << m_fix.getIOPos();
	Message mess;
	mess.reset(m_fix.getIOPos(), "heal", packet, 0.f, false);
	game.getUniverse().getUniverseIO().recieve(mess);
}


