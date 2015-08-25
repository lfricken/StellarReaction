#include "Chunk.hpp"
#include "Module.hpp"
#include "SlaveLocator.hpp"
#include "Controller.hpp"

//Evan - afterburner animation
#include <SFML/Graphics.hpp>
#include "Animator.hpp"

#include <SFML/Audio.hpp>

using namespace std;

Chunk::Chunk(const ChunkData& rData) : GameObject(rData), m_body(rData.bodyComp), m_zoomPool(rData.zoomData), m_energyPool(rData.energyData), m_ballisticPool(rData.ballisticData), m_missilePool(rData.missileData)
{
	PoolCollection myPools;
	myPools.ballisticPool = &m_ballisticPool;
	myPools.zoomPool = &m_zoomPool;
	myPools.missilePool = &m_missilePool;
	myPools.energyPool = &m_energyPool;

	for(auto it = rData.moduleData.begin(); it!=rData.moduleData.end(); ++it)
		m_modules.push_back(sptr<Module>( (*it)->generate(m_body.getBodyPtr(), myPools) ));

	m_slavePosition = game.getUniverse().getSlaveLocator().give(this);

	//TODO - remove commented code
	//QuadComponentData baseDecor;
	//baseDecor.texName = "ship1.png";
	//m_decors.resize(1);

	//Evan -  ship hull sprite
	//add a quadcomponent to m_decors. QuadComponent's are rendered automatically. m_decors will be re-positioned every post-pysics-update
	for (auto it = rData.hullData.begin(); it != rData.hullData.end(); ++it)
	{
		m_decors.push_back(sptr<GraphicsComponent>(new QuadComponent(*it)));
		m_decors.back()->setPosition(m_body.getPosition());
		m_decors.back()->setRotation(m_body.getBodyPtr()->GetAngle());
	}

	//Evan - afterburner animation
	keyDown = false;
	/*sf::Animation walkingAnimationDown;
	walkingAnimationDown.setSpriteSheet(texture);
	walkingAnimationDown.addFrame(sf::IntRect(32, 0, 32, 32));
	walkingAnimationDown.addFrame(sf::IntRect(64, 0, 32, 32));
	walkingAnimationDown.addFrame(sf::IntRect(32, 0, 32, 32));
	walkingAnimationDown.addFrame(sf::IntRect(0,  0, 32, 32));*/

	//Evan - afterburner sound
	buffer.loadFromFile("audio/afterb1.wav");
	afterb_sound.setBuffer(buffer);

}
Chunk::~Chunk()
{
	game.getUniverse().getSlaveLocator().free(m_slavePosition);
}
void Chunk::prePhysUpdate()
{
	for(auto it = m_modules.begin(); it!=m_modules.end(); ++it)
		(*it)->prePhysUpdate();
}
void Chunk::postPhysUpdate()
{
	for(auto it = m_modules.begin(); it!=m_modules.end(); ++it)
		(*it)->postPhysUpdate();

	if(game.getNwBoss().getNWState() == NWState::Server)
	{
		m_nw.toggleNewData(true);
		m_body.getNWComp().toggleNewData(true);
	}

	//Evan - iterate m_decors and rotate (ex: ship hull)
	for (int i = 0; i<(signed)m_decors.size(); ++i)
	{
		m_decors[i]->setPosition(m_body.getPosition());
		m_decors[i]->setRotation(m_body.getBodyPtr()->GetAngle());
	}

	//Evan - W key events (afterburner anim and sound)
	//bool upKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	//if (upKeyPressed)
	//{
	//	cout << keyDown;
	//	if (!keyDown)
	//	{
	//		//start afterburner sound
	//		afterb_sound.play(); 
	//	}
	//	keyDown = true;
	//}
	//else
	//	//stop afterburner sound
	//	afterb_sound.stop();
	//	keyDown = false;


}
const std::string& Chunk::getName() const
{
	return m_io.getName();
}


void Chunk::setAim(const b2Vec2& world)//send our aim coordinates
{
	for(auto it = m_modules.begin(); it!=m_modules.end(); ++it)
		(*it)->setAim(world);
}
void Chunk::directive(Directive issue)//send command to target
{
	for(auto it = m_modules.begin(); it!=m_modules.end(); ++it)
		(*it)->directive(issue);

	//Evan - key press 'up' results in afterburner anim
	 //TODO - better animation handling
	/*if (issue == Directive::Up)
	{
		for (auto it = m_decors.begin(); it != m_decors.end(); ++it)
		{ 
			(*it)->getAnimator().setAnimation("Firing", .25f);
		}
	}*/
}
float Chunk::get(Request value) const//return the requested value
{
	switch(value)
	{
	case(Request::Zoom):
		return m_zoomPool.getValue();
		break;
	case(Request::MaxZoom):
		return m_zoomPool.getMax();
		break;


	case(Request::Energy):
		return m_energyPool.getValue();
		break;
	case(Request::MaxEnergy):
		return m_energyPool.getMax();
		break;


	case(Request::Ballistics):
		return m_ballisticPool.getValue();
		break;
	case(Request::MaxBallistics):
		return m_ballisticPool.getMax();
		break;


	case(Request::Missiles):
		return m_missilePool.getValue();
		break;
	case(Request::MaxMissiles):
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
	cout << "\nCommand not found in [" << m_io.getName() << "].";
}
