#include "ShipModule.hpp"
#include "SoundManager.hpp"
#include "Player.hpp"
#include "Team.hpp"
#include "Grid.hpp"
#include "Weapon.hpp"
#include "Chunk.hpp"
#include "UpgradeType.hpp"


void ShipModuleData::applyUpgrade(UpgradeType type)
{
	if(type == UpgradeType::Armor)
		Upgrade::increase(type, &health.Armor);
	else if(type == UpgradeType::Health)
		Upgrade::increase(type, &health.Max);
	else if(type == UpgradeType::Other)
		for(auto it = rangeModifiers.modifiers.begin(); it != rangeModifiers.modifiers.end(); ++it)
		{
			auto& mod = *it;

			if(mod.Max != 0)
				Upgrade::increase(type, &mod.Max);
			else if(mod.Value != 0)
				Upgrade::increase(type, &mod.Value);
			else if(mod.Min != 0)
				Upgrade::increase(type, &mod.Min);
			else if(mod.ModifierPerSecond != 0)
				Upgrade::increase(type, &mod.ModifierPerSecond);
		}
	else
		WARNING;
}
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
	m_deathSound = rData.deathSound;

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
HealthState ShipModule::getHealthState() const
{
	return m_healthState;
}
void ShipModule::prePhysUpdate()
{
	Module::prePhysUpdate();
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
	if(rCommand == "damage")//TODO huge if statement needs refactor
	{
		int damageAmount;
		int ioPosOfDealer;
		Team team;
		Vec2 hitPoint;
		Vec2 fromDir;
		String effect;
		bool isBleedDamage;
		{
			int teamTemp;
			rData >> damageAmount >> ioPosOfDealer >> teamTemp >> hitPoint.x >> hitPoint.y >> fromDir.x >> fromDir.y >> effect >> isBleedDamage;
			team = static_cast<Team>(teamTemp);
		}

		if(isValidDamageSource(damageAmount, team))//if not same team and valid damage value
		{
			static const float bleedFraction = 2.f; // how much is damage reduced when bleeding?
			int overkill = (int)((damageAmount - m_health.getHealth() - m_health.getArmor()));// / bleedFraction); // how much excess damage there is.

			{//do standard damage
				m_health.damage(damageAmount); //actually do the damage
				changeHealthState(ioPosOfDealer);//potentially award a point
				m_io.event(EventType::Health, m_health.getHealth(), voidPacket);
				moduleDamageGraphics(); //flash hud and module

				if(!isBleedDamage && effect != "") //spark effects
				{
					const Vec2 bodyCenter = m_fix.getBodyPtr()->GetWorldCenter();
					const Vec2 bounce = fromDir.bounce(bodyCenter.to(hitPoint));
					game.getUniverse().spawnParticles(effect, hitPoint, bounce, m_fix.getBodyPtr()->GetLinearVelocity());
				}
			}

			if(overkill > 0)//check if damage should bleed
			{
				ShipModule* newTarget = m_parent->getNearestValidTarget(m_fix.getOffset());//returns null if there are no valid targets

				if(newTarget != nullptr)
				{
					int newModuleTarget = newTarget->m_io.getPosition();//damage it with excess damage
					bool isBleed = true;
					Weapon::damage(&game.getUniverse().getUniverseIO(), newModuleTarget, overkill, ioPosOfDealer, team, hitPoint, fromDir, effect, isBleed);
				}
				else
				{
					Chunk* parent = dynamic_cast<Chunk*>(m_parent);
					parent->explode();

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
		if(!m_health.hasCrits())
			setHealthState(HealthState::Nominal);
	}
	else if(rCommand == "increase_score")
	{
		WARNING;
	}
	else
		Module::input(rCommand, rData);
}
void ShipModule::moduleDamageGraphics()
{
	{// show health damage on ship
		m_decors[m_hitDecorIndex]->getAnimator().setAnimation("Hit", 0.20f);
		m_decors[m_hitDecorIndex]->setColor(m_health.getColor());
	}

	// show health damage on HUD
	auto boardPtr = m_parent->thisAsChunk()->getStatusBoard();
	if(auto statusBoard = boardPtr.lock())
	{
		statusBoard->damageModule(getOffset(), m_healthState, m_health.getHealthPercent(), true);
	}
}
void ShipModule::changeHealthState(int ioPosOfDealer)
{
	if(m_healthState != HealthState::Broken)//only if they transition to being broken should you get points
	{
		if(m_health.hasFullCrits())//this module died this time
		{
			setHealthState(HealthState::Broken);
			sf::Packet pack;
			pack << 1;
			Message mess(ioPosOfDealer, "increase_score", pack, 0.0f, false);
			game.getUniverse().getUniverseIO().recieve(mess);
		}
		else if(m_health.hasCrits())
		{
			setHealthState(HealthState::CriticallyDamaged);
		}
	}
}
bool ShipModule::isValidDamageSource(int damage, Team damagingTeam)
{
	const Team myTeam = m_parent->getBodyComponent().getTeam();
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

	WARNING;
	return true;
}
void ShipModule::setHealthState(HealthState newState)
{
	m_healthState = newState;

	if(m_healthState == HealthState::Nominal)
	{

	}
	else if(m_healthState == HealthState::Broken)
	{
		f_died();
	}

	applyModifiers(isFunctioning());

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

}
void ShipModule::healToMax()
{
	sf::Packet packet;
	packet << m_health.getMaxHealth() << m_fix.getIOPos();
	Message mess;
	mess.reset(m_fix.getIOPos(), "heal", packet, 0.f, false);
	game.getUniverse().getUniverseIO().recieve(mess);
}


