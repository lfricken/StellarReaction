#include "ShipModule.hpp"
#include "SoundManager.hpp"
#include "Player.hpp"
#include "Team.hpp"
#include "Grid.hpp"



void ShipModuleData::loadJson(const Json::Value& root)
{
	LOADJSON(health);
	LOADJSON(baseDecor);
	LOADJSON(deathSound);
	GETJSON(functionsDamaged);

	ModuleData::loadJson(root);
}

ShipModule::ShipModule(const ShipModuleData& rData) : Module(rData), m_health(rData.health)
{
	m_criticalDamageThreshold = 0.25;

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
	data.texName = "effects/module_hit";
	data.layer = GraphicsLayer::ShipAppendagesLower;
	m_decors.push_back(sptr<GraphicsComponent>(new QuadComponent(data)));
	m_hitDecorIndex = m_decors.size() - 1;


	data.dimensions.x = 256;
	data.dimensions.y = 256;
	data.permanentRot = 0;
	data.center.x = 0;
	data.center.y = 0;
	data.texName = "effects/explosion1";
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
	if(rCommand == "damageBleed")//TODO huge if statement needs refactor
	{
		//this damage was already verified

		int damageAmount;
		int ioPosOfDealer;
		Team damagingTeam;
		int teamTemp;
		Vec2 hitPoint;
		Vec2 fromDir;
		String effect;
		rData >> damageAmount >> ioPosOfDealer >> teamTemp >> hitPoint.x >> hitPoint.y >> fromDir.x >> fromDir.y >> effect;

		m_health.damage(damageAmount); //actually do the damage
		changeHealthState(ioPosOfDealer);//potentially award a point
		m_io.event(EventType::Health, m_health.getHealth(), voidPacket);
		moduleDamageGraphics(); //flash hud and module 

	}
	if(rCommand == "damage")//TODO huge if statement needs refactor
	{
		sf::Packet damageCopy(rData);

		int damageAmount;
		int ioPosOfDealer;
		Team damagingTeam;
		int teamTemp;
		Vec2 hitPoint;
		Vec2 fromDir;
		String effect;
		rData >> damageAmount >> ioPosOfDealer >> teamTemp >> hitPoint.x >> hitPoint.y >> fromDir.x >> fromDir.y >> effect;

		if(isValidDamageSource(damageAmount, static_cast<Team>(teamTemp)))//if not same team and valid damage value
		{
			if(m_health.isDead())//if already dead
			{
				Module& newTarget = m_parentChunk->getNearestValidTarget(m_fix.getOffset());
				int newModuleTarget = newTarget.m_io.getPosition();

				moduleDamageGraphics(); //flash hud and module
				
				Message bleed(newModuleTarget, "damageBleed", damageCopy, 0.f, false);
				Message::SendUniverse(bleed);
			}
			else
			{
				m_health.damage(damageAmount); //actually do the damage
				changeHealthState(ioPosOfDealer);//potentially award a point
				m_io.event(EventType::Health, m_health.getHealth(), voidPacket);
				moduleDamageGraphics(); //flash hud and module
				
				if(effect != "") //spark effects
				{
					const Vec2 bodyCenter = m_fix.getBodyPtr()->GetWorldCenter();
					const Vec2 bounce = fromDir.bounce(bodyCenter.to(hitPoint));
					game.getUniverse().spawnParticles(effect, hitPoint, bounce, m_fix.getBodyPtr()->GetLinearVelocity());
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
		if(m_health.getHealthPercent() >= m_criticalDamageThreshold)
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
void ShipModule::moduleDamageGraphics()
{
	{// show health damage on ship
		m_decors[m_hitDecorIndex]->getAnimator().setAnimation("Hit", 0.20f);
		m_decors[m_hitDecorIndex]->setColor(sf::Color(255, static_cast<char>(255.f * m_health.getHealthPercent()), 0, 255));
	}

	// show health damage on HUD
	if(m_parentChunk != nullptr)
	{
		auto pos = getOffset();
		auto boardPtr = m_parentChunk->getStatusBoard();
		if(auto statusBoard = boardPtr.lock())
		{
			statusBoard->damageFlash(pos, m_healthState);
		}
	}
}
void ShipModule::changeHealthState(int ioPosOfDealer)
{
	if(m_healthState != HealthState::Broken)//only if they transition to being broken should you get points
	{
		if(m_health.isDead())//this module died this time
		{
			setHealthState(HealthState::Broken);
			sf::Packet pack;
			pack << 1;
			Message mess(ioPosOfDealer, "increase_score", pack, 0.0f, false);
			game.getUniverse().getUniverseIO().recieve(mess);
		}
		else if(m_health.getHealthPercent() < m_criticalDamageThreshold)
		{
			setHealthState(HealthState::CriticallyDamaged);
		}
	}

}
bool ShipModule::isValidDamageSource(int damage, Team damagingTeam)
{
	const Team myTeam = m_parentChunk->getBodyComponent().getTeam();
	const bool damagePositive = damage > 0;
	const bool differentTeams = damagingTeam != myTeam || damagingTeam == Team::Alone;
	const bool validTeams = (damagingTeam != Team::Invalid && damagingTeam != Team::Capturable) && (myTeam != Team::Invalid && myTeam != Team::Neutral && myTeam != Team::Capturable);

	return (damagePositive && differentTeams && validTeams);
}
bool ShipModule::isFunctioning()//does this module still do its function
{
	if(m_healthState == HealthState::Nominal)
		return true;
	else if(m_healthState == HealthState::Broken)
		return false;
	else if(m_healthState == HealthState::CriticallyDamaged)
		return m_functionsDamaged;

	Print << "\n" << FILELINE;
	return true;
}
void ShipModule::setHealthState(HealthState newState)
{
	m_healthState = newState;

	if(m_healthState == HealthState::Nominal)
	{
		m_decors[m_baseDecor]->setColor(sf::Color(255, 255, 255, 255));///makes the sprite red when destroyed
	}
	else if(m_healthState == HealthState::Broken)
	{
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


