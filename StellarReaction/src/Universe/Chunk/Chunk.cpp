#include "Chunk.hpp"
#include "Module.hpp"
#include "SlaveLocator.hpp"
#include "Controller.hpp"
#include "Overlay.hpp"
#include "BlueprintLoader.hpp"

//Evan - afterburner animation and sound
#include "SoundManager.hpp"
#include <SFML/Audio.hpp>

//Evan - TODO - delete
#include "Animation.hpp"

using namespace std;

Chunk::Chunk(const ChunkData& rData) : GameObject(rData), m_body(rData.bodyComp), m_zoomPool(rData.zoomData), m_energyPool(rData.energyData), m_ballisticPool(rData.ballisticData), m_missilePool(rData.missileData)
{
	m_timer.setCountDown(1);
	m_timer.restartCountDown();
	PoolCollection myPools;
	myPools.ballisticPool = &m_ballisticPool;
	myPools.zoomPool = &m_zoomPool;
	myPools.missilePool = &m_missilePool;
	myPools.energyPool = &m_energyPool;

	m_validOffsets = rData.validPos;

	//TODO: remove this
	for(float i = -5; i < 5; i+=0.5)
		for(float j = -5; j < 5; j += 0.5)
			m_validOffsets.push_back(b2Vec2(i, j));


	for(auto it = rData.moduleData.begin(); it != rData.moduleData.end(); ++it)
		m_modules.push_back(sptr<Module>((*it)->generate(m_body.getBodyPtr(), myPools)));

	m_slavePosition = m_rParent.getSlaveLocator().give(this);

	//TODO - remove commented code
	//QuadComponentData baseDecor;
	//baseDecor.texName = "ship1.png";
	//m_decors.resize(1);

	//Evan -  
	//add a quadcomponent to m_decors. QuadComponent's are rendered automatically. m_decors will be re-positioned every post-physics-update
	//Evan - add hull, afterburner and afterburner_thrust
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
		afterburners_thrust.push_back(temp);
	}

	//Evan - afterburner animation
	keyUpIsdown = false;
	keyShiftIsdown = false;

	//Evan - afterburner sound
	buffer.loadFromFile(contentDir() + "audio/afterb1.wav");
	afterb_sound.setBuffer(buffer);
	afterb_sound.setLoop(true);
	afterb_sound.setVolume(60);

	thrust_buffer.loadFromFile(contentDir() + "audio/afterb2.wav");
	thrust_sound.setBuffer(thrust_buffer);
	thrust_sound.setLoop(true);
	thrust_sound.setVolume(100);

}
Chunk::~Chunk()
{
	m_rParent.getSlaveLocator().free(m_slavePosition);
}
bool Chunk::allows(const b2Vec2& rGridPos)
{
	return (std::find(m_validOffsets.begin(), m_validOffsets.end(), rGridPos) != m_validOffsets.end());
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
		m_modules.push_back(sptr<Module>(rData.generate(m_body.getBodyPtr(), myPools)));
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
	for(auto it = afterburners_thrust.begin(); it != afterburners_thrust.end(); ++it)
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

	//Evan - key press 'up' results in afterburner anim
	//TODO - leon needs to make chunk vars available to thruster module (need to set anim for hull etc)
	bool upKeyPressed = rIssues[Directive::Up];
	bool shiftKeyPressed = rIssues[Directive::Boost];
	if(upKeyPressed)
	{
		if(!keyUpIsdown)
		{
			hull->getAnimator().setAnimation("AfterBurner", .35f);
			for(auto it = afterburners.begin(); it != afterburners.end(); ++it)
			{
				(*it)->getAnimator().setAnimation("AfterBurner", .20f);
			}
		}
	}
	else
	{
		if(keyUpIsdown)
		{
			hull->getAnimator().setAnimation("Default", .20f);
			for(auto it = afterburners.begin(); it != afterburners.end(); ++it)
			{
				(*it)->getAnimator().setAnimation("Default", .20f);
			}
		}
	}

	//Evan - enable thruster anim on shift key press
	if(shiftKeyPressed && upKeyPressed)
	{
		if(!keyShiftIsdown || !keyUpIsdown)
		{

			for(auto it = afterburners.begin(); it != afterburners.end(); ++it)
			{
				(*it)->getAnimator().setAnimation("Default", .20f);
			}
			for(auto it = afterburners_thrust.begin(); it != afterburners_thrust.end(); ++it)
			{
				(*it)->getAnimator().setAnimation("Thrust", .20f);
			}

			//add velocity to ship - add to thruster anim
			thrust_sound.play();
		}
	}
	else
	{
		if(keyShiftIsdown)
		{

			for(auto it = afterburners_thrust.begin(); it != afterburners_thrust.end(); ++it)
			{
				(*it)->getAnimator().setAnimation("Default", .20f);
			}

			if(upKeyPressed)
			{
				for(auto it = afterburners.begin(); it != afterburners.end(); ++it)
				{
					(*it)->getAnimator().setAnimation("AfterBurner", .20f);
				}
			}
			thrust_sound.stop();
		}
	}

	//Evan - W key events (afterburner anim and sound)	
	//= sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	if(upKeyPressed)
	{
		if(!keyUpIsdown)
		{
			//start afterburner sound
			//game.getSound().playSound("afterb1.wav"); // , 100, 15.f, 0.f, new b2Vec2(0, 0), false);
			afterb_sound.play();
		}
	}
	else
	{
		//stop afterburner sound
		if(keyUpIsdown)
		{
			afterb_sound.stop();
		}
	}


	//Evan - set keyDown
	//used to trigger that should only happen once when key is pressed and once when key is released
	if(upKeyPressed)
		keyUpIsdown = true;
	else
		keyUpIsdown = false;

	if(shiftKeyPressed)
		keyShiftIsdown = true;
	else
		keyShiftIsdown = false;
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
