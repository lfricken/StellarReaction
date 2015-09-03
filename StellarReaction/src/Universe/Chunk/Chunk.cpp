#include "Chunk.hpp"
#include "Module.hpp"
#include "SlaveLocator.hpp"
#include "Controller.hpp"
#include "Overlay.hpp"

//Evan - afterburner animation and sound
#include "SoundManager.hpp"
#include <SFML/Audio.hpp>

//Evan - TODO - delete
#include "Animation.hpp"

using namespace std;

Chunk::Chunk(const ChunkData& rData) : GameObject(rData), m_body(rData.bodyComp), m_zoomPool(rData.zoomData), m_energyPool(rData.energyData), m_ballisticPool(rData.ballisticData), m_missilePool(rData.missileData)
{
	PoolCollection myPools;
	myPools.ballisticPool = &m_ballisticPool;
	myPools.zoomPool = &m_zoomPool;
	myPools.missilePool = &m_missilePool;
	myPools.energyPool = &m_energyPool;

	for(auto it = rData.moduleData.begin(); it != rData.moduleData.end(); ++it)
		m_modules.push_back(sptr<Module>((*it)->generate(m_body.getBodyPtr(), myPools)));

	m_slavePosition = game.getUniverse().getSlaveLocator().give(this);

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
	buffer.loadFromFile("audio/afterb1.wav");
	afterb_sound.setBuffer(buffer);
	afterb_sound.setLoop(true);
	afterb_sound.setVolume(60);

	thrust_buffer.loadFromFile("audio/afterb2.wav");
	thrust_sound.setBuffer(thrust_buffer);
	thrust_sound.setLoop(true);
	thrust_sound.setVolume(100);

}
void Chunk::add(const ModuleData& rData)
{
	PoolCollection myPools;
	myPools.ballisticPool = &m_ballisticPool;
	myPools.zoomPool = &m_zoomPool;
	myPools.missilePool = &m_missilePool;
	myPools.energyPool = &m_energyPool;
	m_modules.push_back(sptr<Module>(rData.generate(m_body.getBodyPtr(), myPools)));
}
Chunk::~Chunk()
{
	game.getUniverse().getSlaveLocator().free(m_slavePosition);
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
b2Body* Chunk::getBodyPtr()
{
	return m_body.getBodyPtr();
}
void Chunk::input(std::string rCommand, sf::Packet rData)
{
	if(rCommand == "addShipModule")
	{

	}
	cout << "\nCommand not found in [" << m_io.getName() << "].";
}
