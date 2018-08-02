#include "Chunk.hpp"
#include "Module.hpp"
#include "SlaveLocator.hpp"
#include "Controller.hpp"
#include "Overlay.hpp"
#include "SoundManager.hpp"
#include "Animation.hpp"
#include "ShipModule.hpp"
#include "CommandInfo.hpp"
#include "Shield.hpp"
#include "Resources.hpp"
#include "BlueprintLoader.hpp"


void ChunkDataMessage::loadJson(const Json::Value& root)
{
	GETJSON(cost);
	GETJSON(blueprintName);
	GETJSON(coordinates);
	GETJSON(rotation);
	GETJSON(team);
	GETJSON(needsController);
	GETJSON(aiControlled);

	if(aiControlled)
		needsController = true;
}
void ChunkDataMessage::pack(sf::Packet* data) const
{
	sf::Packet& pack = *data;
	pack << blueprintName;
	pack << coordinates.x << coordinates.y;
	pack << rotation;
	pack << team;
	pack << slaveName;
	pack << needsController;
	pack << aiControlled;
}
void ChunkDataMessage::unpack(const sf::Packet& data)
{
	sf::Packet pack = data;
	pack >> blueprintName;
	pack >> coordinates.x >> coordinates.y;
	pack >> rotation;
	pack >> team;
	pack >> slaveName;
	pack >> needsController;
	pack >> aiControlled;
}
void ChunkData::loadJson(const Json::Value& root)
{
	ModuleParentData::loadJson(root);

	GETJSON(minShieldPower);
	LOADJSON(ioComp);
	LOADJSON(teleporterComp);

	LOADJSON(hullSpriteData);

	if(!root["afterburnerSpriteData"].isNull())
		for(auto it = root["afterburnerSpriteData"].begin(); it != root["afterburnerSpriteData"].end(); ++it)
		{
			QuadComponentData quad;
			quad.loadJson(*it);
			afterburnerSpriteData.push_back(quad);
		}

	if(!root["afterburnerThrustSpriteData"].isNull())
		for(auto it = root["afterburnerThrustSpriteData"].begin(); it != root["afterburnerThrustSpriteData"].end(); ++it)
		{
			QuadComponentData quad;
			quad.loadJson(*it);
			afterburnerThrustSpriteData.push_back(quad);
		}
}
Chunk::Chunk(const ChunkData& data) : ModuleParent(data), m_io(data.ioComp, &Chunk::input, this), m_universe(*data.universeParent), m_teleporter(data.teleporterComp, this)
{
	m_inDeathProcess = false;
	m_canDie = true;
	m_shieldToggleTimer.setCountDown(0.5f);
	m_shieldToggleTimer.restartCountDown();

	m_spawnPoint = data.bodyComp.coords;
	m_radius = -1.f;
	m_wasThrusting = false;
	m_wasBoosting = false;
	m_stealth = false;
	m_timer.setCountDown(1);
	m_timer.restartCountDown();
	m_areShieldsOn = false;
	m_minShieldPower = data.minShieldPower;


	//Hull Sprite
	hull = sptr<GraphicsComponent>(new QuadComponent(data.hullSpriteData));
	//Thrust sprite.
	for(auto it = data.afterburnerSpriteData.begin(); it != data.afterburnerSpriteData.end(); ++it)
	{
		sptr<GraphicsComponent> temp = sptr<GraphicsComponent>(new QuadComponent(*it));
		afterburners.push_back(temp);
	}
	//Boost sprite.
	for(auto it = data.afterburnerThrustSpriteData.begin(); it != data.afterburnerThrustSpriteData.end(); ++it)
	{
		sptr<GraphicsComponent> temp = sptr<GraphicsComponent>(new QuadComponent(*it));
		afterburners_boost.push_back(temp);
	}

	//For sounds.
	m_thrustNoiseIndex = -1;
	m_boostNoiseIndex = -1;

}
Chunk::~Chunk()
{
	if(m_controller != -1)//this ship might not have a controller or ai
		m_universe.getControllerFactory().m_list.freeThis(m_controller);
	if(m_shipAI != -1)
		m_universe.getShipAI().freeThis(m_shipAI);
}
Vec2 Chunk::getSpawn()
{
	return m_spawnPoint;
}
Vec2 Chunk::getClearSpawn()
{
	if(getGame()->getUniverse().isClear(m_spawnPoint, getRadius(), this))
		return m_spawnPoint;
	else
		return getGame()->getUniverse().getAvailableSpawn(m_body.getTeam(), getRadius(), this);
}
void Chunk::setSpawn(float x, float y)
{
	m_spawnPoint = Vec2(x, y);
}
void Chunk::setStealth(bool stealthToggle)
{
	m_stealth = stealthToggle;
}
bool Chunk::isStealth()
{
	return m_stealth;
}
Vec2 Chunk::getAim() const
{
	return m_lastAim;
}
sptr<GraphicsComponent> Chunk::getHull() const
{
	return hull;
}
void Chunk::prePhysUpdate()
{
	//TODO 
	const int forceMult = 10;

	//push chunk in bounds if out of bounds
	Vec2 location = m_body.getPosition();
	Vec2 bounds = getGame()->getUniverse().getBounds();
	if(abs(location.x) > bounds.x || abs(location.y) > bounds.y)
	{
		Vec2 force = Vec2(-location.x, -location.y);
		float diff = std::max(abs(location.x) - bounds.x, abs(location.y) - bounds.y);
		force = force.unit();
		force *= forceMult * diff;
		m_body.applyForce(force);
	}

	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		(*it)->prePhysUpdate();
}
void Chunk::postPhysUpdate()
{
	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		(*it)->postPhysUpdate();

	m_body.getNWComp().toggleNewData(true);//update our physics self


	const auto pos = m_body.getPosition();
	const auto angle = m_body.getAngle();

	hull->setPosition(pos);
	hull->setRotation(angle);
	for each (auto image in afterburners)
	{
		image->setPosition(pos);
		image->setRotation(angle);
	}
	for each (auto image in afterburners_boost)
	{
		image->setPosition(pos);
		image->setRotation(angle);
	}
}
void Chunk::setAim(const Vec2& world)//send our aim coordinates
{
	m_lastAim = world;
	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		(*it)->setAim(m_lastAim);
}
void Chunk::directive(const CommandInfo& commands)//send command to target
{
	std::map<Directive, bool> rIssues = commands.directives;
	bool local = commands.isLocal;

	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		(*it)->directive(commands);

	if(rIssues[Directive::ShowStore] && local)
	{
		if(m_timer.isTimeUp())
		{
			String store = "store_default";

			Message toggle(store, "toggleHidden", voidPacket, 0, false);
			Message mes2("local_player", "toggleGuiMode", voidPacket, 0, false);
			getGame()->getCoreIO().recieve(toggle);
			getGame()->getCoreIO().recieve(mes2);

			m_timer.restartCountDown();
		}
	}

	bool startThrusting = (rIssues[Directive::Up] && !m_wasThrusting);
	bool startBoosting = (rIssues[Directive::Up] && rIssues[Directive::Boost] && !m_wasBoosting);


	//Shields
	if(rIssues[Directive::Shield] && m_shieldToggleTimer.isTimeUp())
	{
		m_shieldToggleTimer.restartCountDown();
		Message shield;

		bool enoughEnergy = m_ranges[RangeList::Energy].getValue() > 0.25f;
		if(!m_areShieldsOn && enoughEnergy)//if they aren't already on and we have enough energy turn them on
			shield.reset(m_io.getPosition(), "enableShields", voidPacket, 0, false);
		else if(m_areShieldsOn)//if they are on turn them off no mattter what.
			shield.reset(m_io.getPosition(), "disableShields", voidPacket, 0, false);

		Message::SendUniverse(shield);
	}


	if(startThrusting)
	{
		for(auto it = afterburners.begin(); it != afterburners.end(); ++it)
			(*it)->getAnimator().setAnimation("Activated", 0.20f);
		//Sound needed.
		//m_thrustNoiseIndex = getGame()->getSound().playSound("Afterburner.wav", 100, 20, 2, getBodyPtr()->GetPosition(), true, true);
	}
	if(startBoosting)
	{
		for(auto it = afterburners_boost.begin(); it != afterburners_boost.end(); ++it)
			(*it)->getAnimator().setAnimation("Activated", 0.20f);
		//Sound needed.
		//m_boostNoiseIndex = getGame()->getSound().playSound("Afterburner.wav", 100, 20, 2, getBodyPtr()->GetPosition(), true, true);
	}



	if(!rIssues[Directive::Up])
	{
		for(auto it = afterburners.begin(); it != afterburners.end(); ++it)
			(*it)->getAnimator().setAnimation("Default", 1.0f);
		//Sound needed.
		//m_thrustNoiseIndex = getGame()->getSound().stopSound(m_thrustNoiseIndex);
	}
	if(!rIssues[Directive::Up] || !rIssues[Directive::Boost])
	{
		for(auto it = afterburners_boost.begin(); it != afterburners_boost.end(); ++it)
			(*it)->getAnimator().setAnimation("Default", 1.0f);
		//Sound needed.
		//m_boostNoiseIndex = getGame()->getSound().stopSound(m_boostNoiseIndex);
	}

	m_wasThrusting = rIssues[Directive::Up];
	m_wasBoosting = (rIssues[Directive::Up] && rIssues[Directive::Boost]);

	m_teleporter.directive(commands);
}
float Chunk::get(Request value) const//return the requested value
{
	switch(value)
	{
	case(Request::Zoom) :
		return static_cast<float>(m_ranges[RangeList::Zoom].getValue());
	case(Request::MaxZoom) :
		return static_cast<float>(m_ranges[RangeList::Zoom].getMax());


	case(Request::Energy) :
		return static_cast<float>(m_ranges[RangeList::Energy].getValue());
	case(Request::MaxEnergy) :
		return static_cast<float>(m_ranges[RangeList::Energy].getMax());


	case(Request::Ballistics) :
		return static_cast<float>(m_ranges[RangeList::Ballistic].getValue());
	case(Request::MaxBallistics) :
		return static_cast<float>(m_ranges[RangeList::Ballistic].getMax());


	case(Request::Missiles) :
		return static_cast<float>(m_ranges[RangeList::Missiles].getValue());
	case(Request::MaxMissiles) :
		return static_cast<float>(m_ranges[RangeList::Missiles].getMax());

	case(Request::ShieldState) :
		return static_cast<float>(m_areShieldsOn);


	default:
		return 0.f;
		break;
	}
}
void Chunk::explode()
{
	if(m_canDie && !m_inDeathProcess)
	{
		m_inDeathProcess = true;
		removeFromUniverse(this->getFactoryPosition(), true, 0.1f);

		//TODO death effects group
		{ // death effects
			getGame()->getUniverse().spawnParticles("SparkExplosion", getBodyComponent().getPosition(), Vec2(1, 0), Vec2(0, 0));
			getGame()->getUniverse().spawnParticles("WhiteFlash", getBodyComponent().getPosition(), Vec2(1, 0), Vec2(0, 0));
			getGame()->getUniverse().spawnParticles("DebrisExplosion1", getBodyComponent().getPosition(), Vec2(1, 0), Vec2(0, 0));
			getGame()->getUniverse().spawnParticles("DebrisExplosion2", getBodyComponent().getPosition(), Vec2(1, 0), Vec2(0, 0));
			getGame()->getUniverse().spawnParticles("DebrisExplosion3", getBodyComponent().getPosition(), Vec2(1, 0), Vec2(0, 0));
		}


		{ // create loot
			ChunkDataMessage ship;

			ship.aiControlled = false;
			ship.needsController = false;
			ship.coordinates = getBodyComponent().getPosition();
			ship.blueprintName = "Loot";
			ship.rotation = 0;
			ship.team = static_cast<int>(Team::Neutral);
			ShipBuilder::Server::createChunk(ship, 0.3f);
		}
	}
}
void Chunk::removeFromUniverse(int targetChunkUniversePos, bool shake, float delay)
{
	sf::Packet myPosData;
	myPosData << targetChunkUniversePos;
	myPosData << shake;
	Message death("universe", "killChunkCommand", myPosData, delay, false);
	Message::SendUniverse(death);
}
void Chunk::input(String rCommand, sf::Packet rData)
{
	if(rCommand == "clear")
	{
		this->clear();
	}
	else if(rCommand == "pickupLoot")
	{
		Resources loot;
		int lootChunkUniversePos;

		loot.fromPacket(&rData);
		rData >> lootChunkUniversePos;

		Resources::ChangeResources(loot, m_body.getTeam());

		if(lootChunkUniversePos != -1)
			removeFromUniverse(lootChunkUniversePos, false, 0.f);
	}
	else if(rCommand == "doneBuilding")
	{
		resetStatusBoard(getStatusBoard());
	}
	else if(rCommand == "attachModule")
	{
		String bpName;
		sf::Vector2i pos;

		rData >> bpName;
		rData >> pos.x;
		rData >> pos.y;

		Vec2 offset = (Vec2)pos;

		auto pNewModuleData = sptr<ModuleData>(m_universe.getBlueprints().getModuleSPtr(bpName)->clone());
		if(pNewModuleData != nullptr)
		{
			this->add(bpName, offset);
		}
		else
		{
			Print << "\nBlueprint didn't exist.";
		}
	}
	else if(rCommand == "detachModule")
	{
		sf::Vector2i pos;//the offset we are looking to remove

		rData >> pos.x;
		rData >> pos.y;

		Vec2 targetOffset((float)pos.x, (float)pos.y);

		bool found = false;
		for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		{
			if(targetOffset == (*it)->getOffset())
			{
				found = true;
				m_modules.erase(it);
				break;
			}
		}
		if(!found)
			Print << "\nThere was no module at " << targetOffset.x << "," << targetOffset.y << " " << FILELINE;
	}
	else if(rCommand == "enableShields")
	{
		m_areShieldsOn = true;
		for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		{
			Shield* pShield = dynamic_cast<Shield*>(it->get());
			if(pShield)
				pShield->enableShield();
		}
	}
	else if(rCommand == "disableShields")
	{
		m_areShieldsOn = false;
		for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		{
			Shield* pShield = dynamic_cast<Shield*>(it->get());
			if(pShield)
				pShield->disableShield();
		}
	}
	else
		Print << "\nCommand not found in [" << m_io.getName() << "]." << FILELINE;
}



