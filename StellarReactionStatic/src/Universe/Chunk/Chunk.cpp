#include "Chunk.hpp"
#include "Module.hpp"
#include "SlaveLocator.hpp"
#include "Controller.hpp"
#include "Overlay.hpp"
#include "BlueprintLoader.hpp"
#include "SoundManager.hpp"
#include "Animation.hpp"

using namespace std;

Chunk::Chunk(const ChunkData& rData) : GameObject(rData), m_body(rData.bodyComp), m_zoomPool(rData.zoomData), m_energyPool(rData.energyData), m_ballisticPool(rData.ballisticData), m_missilePool(rData.missileData)
{
	m_wasThrusting = false;
	m_wasBoosting = false;
	m_timer.setCountDown(1);
	m_timer.restartCountDown();
	PoolCollection myPools;
	myPools.ballisticPool = &m_ballisticPool;
	myPools.zoomPool = &m_zoomPool;
	myPools.missilePool = &m_missilePool;
	myPools.energyPool = &m_energyPool;

	m_validOffsets = rData.validPos;


	for(auto it = rData.moduleData.begin(); it != rData.moduleData.end(); ++it)
		m_modules.push_back(sptr<Module>((*it)->generate(m_body.getBodyPtr(), myPools, this)));

	m_slavePosition = m_rParent.getSlaveLocator().give(this);



	hull = sptr<GraphicsComponent>(new QuadComponent(rData.hullSpriteData));
	hull->setPosition(m_body.getPosition());
	hull->setRotation(m_body.getBodyPtr()->GetAngle());
	for(auto it = rData.afterburnerSpriteData.begin(); it != rData.afterburnerSpriteData.end(); ++it)
	{
		sptr<GraphicsComponent> temp = sptr<GraphicsComponent>(new QuadComponent(*it));
		temp->setPosition(m_body.getPosition());
		temp->setRotation(m_body.getBodyPtr()->GetAngle());
		afterburners.push_back(temp);
	}
	for(auto it = rData.afterburnerThrustSpriteData.begin(); it != rData.afterburnerThrustSpriteData.end(); ++it)
	{
		sptr<GraphicsComponent> temp = sptr<GraphicsComponent>(new QuadComponent(*it));
		temp->setPosition(m_body.getPosition());
		temp->setRotation(m_body.getBodyPtr()->GetAngle());
		afterburners_boost.push_back(temp);
	}

	m_thrustNoiseIndex = -1;
	m_boostNoiseIndex = -1;
	m_body.setTeam(rData.team);
}
Chunk::~Chunk()
{
	m_rParent.getSlaveLocator().free(m_slavePosition);
}
sptr<GraphicsComponent> Chunk::getHull() const
{
	return hull;
}
std::vector<sptr<Module>> Chunk::getModuleList() const
{
	return m_modules;
}
bool Chunk::allows(const b2Vec2& rGridPos)
{
	return (std::find(m_validOffsets.begin(), m_validOffsets.end(), rGridPos) != m_validOffsets.end());
}
BodyComponent& Chunk::getBodyComponent()
{
	return m_body;
}
void Chunk::add(const ModuleData& rData)
{
	if(this->allows(rData.fixComp.offset))
	{
		PoolCollection myPools;
		myPools.ballisticPool = &m_ballisticPool;
		myPools.zoomPool = &m_zoomPool;
		myPools.missilePool = &m_missilePool;
		myPools.energyPool = &m_energyPool;
		m_modules.push_back(sptr<Module>(rData.generate(m_body.getBodyPtr(), myPools, this)));
	}
	else
		cout << FILELINE;
}
void Chunk::prePhysUpdate()
{
	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		(*it)->prePhysUpdate();
}
void Chunk::postPhysUpdate()
{
	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		(*it)->postPhysUpdate();

	m_nw.toggleNewData(true);
	m_body.getNWComp().toggleNewData(true);

	//Evan - rotate hull, afterburner and afterburner_thrust
	hull->setPosition(m_body.getPosition());
	hull->setRotation(m_body.getBodyPtr()->GetAngle());
	for(auto it = afterburners.begin(); it != afterburners.end(); ++it)
	{
		(*it)->setPosition(m_body.getPosition());
		(*it)->setRotation(m_body.getBodyPtr()->GetAngle());
	}
	for(auto it = afterburners_boost.begin(); it != afterburners_boost.end(); ++it)
	{
		(*it)->setPosition(m_body.getPosition());
		(*it)->setRotation(m_body.getBodyPtr()->GetAngle());
	}
}
const std::string& Chunk::getName() const
{
	return m_io.getName();
}
void Chunk::setAim(const b2Vec2& world)//send our aim coordinates
{
	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		(*it)->setAim(world);
}
void Chunk::directive(std::map<Directive, bool>& rIssues, bool local)//send command to target
{
	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		(*it)->directive(rIssues);

	if(rIssues[Directive::ShowStore] && local)
	{
		if(m_timer.isTimeUp())
		{
			std::string store;
			for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
			{
				store = (*it)->getStore();
				if(store != "")
					break;
			}
			if(store != "")
			{
				Message toggle(store, "toggleHidden", voidPacket, 0, false);
				Message mes2("local_player", "toggleGuiMode", voidPacket, 0, false);
				game.getCoreIO().recieve(toggle);
				game.getCoreIO().recieve(mes2);
			}
			m_timer.restartCountDown();
		}
	}

	bool startThrusting = (rIssues[Directive::Up] && !m_wasThrusting);
	bool startBoosting = (rIssues[Directive::Up] && rIssues[Directive::Boost] && !m_wasBoosting);


	if(startThrusting)
	{
		for(auto it = afterburners.begin(); it != afterburners.end(); ++it)
			(*it)->getAnimator().setAnimation("Thrust", 0.20f);
		m_thrustNoiseIndex = game.getSound().playSound("afterb1.wav", 60, 20, 20, getBodyPtr()->GetWorldCenter(), true, true);
	}

	if(startBoosting)
	{
		for(auto it = afterburners_boost.begin(); it != afterburners_boost.end(); ++it)
			(*it)->getAnimator().setAnimation("Boost", 0.20f);
		m_boostNoiseIndex = game.getSound().playSound("afterb2.wav", 100, 20, 20, getBodyPtr()->GetWorldCenter(), true, true);
	}


	if(!rIssues[Directive::Up])
	{
		for(auto it = afterburners.begin(); it != afterburners.end(); ++it)
			(*it)->getAnimator().setAnimation("Default", 1.0f);
		m_thrustNoiseIndex = game.getSound().stopSound(m_thrustNoiseIndex);
	}

	if(!rIssues[Directive::Up] || !rIssues[Directive::Boost])
	{
		for(auto it = afterburners_boost.begin(); it != afterburners_boost.end(); ++it)
			(*it)->getAnimator().setAnimation("Default", 1.0f);
		m_boostNoiseIndex = game.getSound().stopSound(m_boostNoiseIndex);
	}

	m_wasThrusting = rIssues[Directive::Up];
	m_wasBoosting = (rIssues[Directive::Up] && rIssues[Directive::Boost]);

}
float Chunk::get(Request value) const//return the requested value
{
	switch(value)
	{
	case(Request::Zoom) :
		return m_zoomPool.getValue();
		break;
	case(Request::MaxZoom) :
		return m_zoomPool.getMax();
		break;


	case(Request::Energy) :
		return m_energyPool.getValue();
		break;
	case(Request::MaxEnergy) :
		return m_energyPool.getMax();
		break;


	case(Request::Ballistics) :
		return m_ballisticPool.getValue();
		break;
	case(Request::MaxBallistics) :
		return m_ballisticPool.getMax();
		break;


	case(Request::Missiles) :
		return m_missilePool.getValue();
		break;
	case(Request::MaxMissiles) :
		return m_missilePool.getMax();
		break;
	default:
		return 0.f;
		break;
	}
}
/// <summary>
/// returns the title of the module at that position, otherwise ""
/// </summary>
std::string Chunk::hasModuleAt(const b2Vec2 offset) const
{
	for(auto it = m_modules.begin(); it != m_modules.end(); ++it)
		if(offset == (*it)->getOffset())
			return (*it)->getTitle();
	return "";
}
/// <summary>
/// List of module BP names and 
/// </summary>
std::vector<std::pair<std::string, b2Vec2> > Chunk::getModules() const
{
	std::vector<std::pair<std::string, b2Vec2> > list;
	for(auto it = m_modules.cbegin(); it != m_modules.cend(); ++it)
	{
		cout << "\nChunk: " << (*it)->getOffset().x << (*it)->getOffset().y;
		list.push_back(std::pair<std::string, b2Vec2>((*it)->getTitle(), b2Vec2((*it)->getOffset())));
	}
	return list;
}
b2Body* Chunk::getBodyPtr()
{
	return m_body.getBodyPtr();
}
void Chunk::clear()
{
	m_modules.clear();
}
void Chunk::input(std::string rCommand, sf::Packet rData)
{
	if(rCommand == "clear")
	{
		this->clear();
	}
	else if(rCommand == "attachModule")
	{
		std::string bpName;
		float x;
		float y;

		rData >> bpName;
		rData >> x;
		rData >> y;

		sptr<ModuleData> pNewModuleData = sptr<ModuleData>(m_rParent.getBlueprints().getModuleSPtr(bpName)->clone());
		if(pNewModuleData != NULL)
		{
			pNewModuleData->fixComp.offset = b2Vec2(x, y);
			this->add(*pNewModuleData);
		}
		else
		{
			cout << "\nBlueprint didn't exist." << FILELINE;
		}
	}
	else if(rCommand == "detachModule")
	{
		float x;
		float y;

		rData >> x;
		rData >> y;

		b2Vec2 targetOffset(x, y);//the offset we are looking to remove

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
			cout << "\nThere was no module at " << x << "," << y << " " << FILELINE;
	}
	else
		cout << "\nCommand not found in [" << m_io.getName() << "]." << FILELINE;
}
void ChunkData::loadJson(const Json::Value& root)
{
	if(!root["Copies"].isNull())
		*this = *dynamic_cast<const ChunkData*>(game.getUniverse().getBlueprints().getChunkSPtr(root["Copies"].asString()).get());

	if(!root["IO"].isNull())
		ioComp.loadJson(root["IO"]);
	if(!root["Network"].isNull())
		nwComp.loadJson(root["Network"]);
	if(!root["Body"].isNull())
		bodyComp.loadJson(root["Body"]);
	if(!root["Missiles"].isNull())
		missileData.loadJson<Missiles>(root["Missiles"]);
	if(!root["Ballistic"].isNull())
		ballisticData.loadJson<Ballistic>(root["Ballistic"]);
	if(!root["Energy"].isNull())
		energyData.loadJson<Energy>(root["Energy"]);
	if(!root["Zoom"].isNull())
		zoomData.loadJson<Zoom>(root["Zoom"]);

	if(!root["Hull_Sprite"].isNull())
		hullSpriteData.loadJson(root["Hull_Sprite"]);

	if(!root["Afterburner_Sprites"].isNull())
		for(auto it = root["Afterburner_Sprites"].begin(); it != root["Afterburner_Sprites"].end(); ++it)
		{
			QuadComponentData quad;
			quad.loadJson(*it);
			afterburnerSpriteData.push_back(quad);
		}

	if(!root["Afterburner_Thrust_Sprites"].isNull())
		for(auto it = root["Afterburner_Thrust_Sprites"].begin(); it != root["Afterburner_Thrust_Sprites"].end(); ++it)
		{
			QuadComponentData quad;
			quad.loadJson(*it);
			afterburnerThrustSpriteData.push_back(quad);
		}

	if(!root["Modules"].isNull())
	{
		sptr<ModuleData> spMod;
		for(auto it = root["Modules"].begin(); it != root["Modules"].end(); ++it)
		{
			if(!(*it)["Title"].isNull() && (*it)["ClassName"].isNull())//from title
			{
				spMod.reset(game.getUniverse().getBlueprints().getModuleSPtr((*it)["Title"].asString())->clone());

				spMod->fixComp.offset.x = (*it)["Position"][0].asFloat();
				spMod->fixComp.offset.y = (*it)["Position"][1].asFloat();
			}
			else if(!(*it)["ClassName"].isNull())//from inline
			{
				spMod.reset(game.getUniverse().getBlueprints().loadModule(*it)->clone());
			}
			else
			{
				cout << "\n" << FILELINE;
				///ERROR LOG
			}

			moduleData.push_back(spMod);
		}
	}
}

