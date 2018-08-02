#include "Projectile.hpp"
#include "ProjectileMan.hpp"
#include "Game.hpp"
#include "BlueprintLoader.hpp"



void ProjectileData::loadJson(const Json::Value& root)
{
	ModuleParentData::loadJson(root);

	GETJSON(lifetime);
}
Projectile::Projectile(const ProjectileData& data) : ModuleParent(data)
{
	m_inPlay = false;
	m_timer.setCountDown(data.lifetime);
}
Projectile::~Projectile()
{

}
void Projectile::launch(const LaunchData& data)
{
	m_inPlay = true;
	m_timer.setCountDown(data.lifetime);
	m_timer.restartCountDown();
	m_body.wake(data.startPosition, data.rotation, data.velocity, data.rotationRate);

	setPayloadOnModules(data.damage, data.team, data.launcherModuleIoPosition, data.collisions);
}
void Projectile::setPayloadOnModules(int damage, Team team, int launcherModuleIoPosition, int collisions)
{
	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
	{
		ProjectileModule* mod = dynamic_cast<ProjectileModule*>(it->get());
		if(mod != nullptr)
			mod->setPayload(damage, team, launcherModuleIoPosition, collisions);
		else
			WARNING;
	}
}
void Projectile::reset()
{
	m_inPlay = false;
	getGame()->getUniverse().getProjMan().freeProjectile(this);
	m_body.sleep();
}
void Projectile::prePhysUpdate()
{
	if(m_inPlay)
		for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
			(*it)->prePhysUpdate();
	//update hull
}
void Projectile::postPhysUpdate()
{
	if(m_inPlay)
	{
		//check to see if we should terminate
		bool endThyself = false;

		if(m_timer.isTimeUp())
			endThyself = true;
		else
			for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
			{
				ProjectileModule* mod = dynamic_cast<ProjectileModule*>(it->get());
				if(mod != nullptr)
				{
					if(mod->shouldTerminate())
						endThyself = true;
				}
				else
				{
					WARNING;
				}
			}

		if(endThyself)
			reset();

		//update graphics
		for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
			(*it)->postPhysUpdate();
	}
	//update hull
}







void Projectile::LaunchData::fromPacket(sf::Packet* data)
{
	sf::Packet& packet = *data;
	int tempTeam;

	packet >> blueprint;
	packet >> targetChunkPosition;
	packet >> launcherModuleIoPosition;

	packet >> startPosition.x << startPosition.y;

	packet >> rotation;
	packet >> rotationRate;
	packet >> velocity.x << velocity.y;
	packet >> maxVelocity;
	packet >> acceleration;

	packet >> tempTeam;
	packet >> lifetime;
	packet >> damage;
	packet >> collisions;

	team = static_cast<Team>(tempTeam);
}
void Projectile::LaunchData::intoPacket(sf::Packet* data) const
{
	sf::Packet& packet = *data;

	packet << blueprint;
	packet << targetChunkPosition;
	packet << launcherModuleIoPosition;

	packet << startPosition.x << startPosition.y;

	packet << rotation;
	packet << rotationRate;
	packet << velocity. x << velocity.y;
	packet << maxVelocity;
	packet << acceleration;

	packet << static_cast<int>(team);
	packet << lifetime;
	packet << damage;
	packet << collisions;
}
sptr<sf::Packet> Projectile::LaunchData::intoPacket() const
{
	sptr<sf::Packet> packet(new sf::Packet);
	intoPacket(packet.get());
	return packet;
}
