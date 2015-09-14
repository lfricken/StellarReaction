#include "ShipModule.hpp"
#include "SoundManager.hpp"

using namespace std;

ShipModule::ShipModule(const ShipModuleData& rData) : Module(rData), m_health(rData.health)
{
	m_decors.resize(1);
	m_decors[0] = sptr<GraphicsComponent>(new QuadComponent(rData.baseDecor));

	m_healthState = rData.initHealthState;
	m_functionsDamaged = rData.functionsDamaged;
	m_decors[0]->setPosition(m_fix.getCenter());
	m_decors[0]->setRotation(m_fix.getAngle());

	//Evan - load 'hit' sprite/animation
	QuadComponentData data;
	data.dimensions.x = 256;
	data.dimensions.y = 256;
	data.permanentRot = 0;
	data.center.x = 0;
	data.center.y = 0;
	data.texName = "hulls/hit1.png";
	data.animSheetName = "hulls/hit1.acfg";
	data.layer = GraphicsLayer::ShipAppendagesLower;
	hitAnim = sptr<GraphicsComponent>(new QuadComponent(data));
	hitAnim->setPosition(m_fix.getCenter());
	hitAnim->setRotation(m_fix.getAngle());



	//Evan - load 'explosion' sprite/animation
	data = QuadComponentData();
	data.dimensions.x = 512;
	data.dimensions.y = 512;
	data.permanentRot = 0;
	data.center.x = 0;
	data.center.y = 0;
	data.texName = "hulls/explosion1.png";
	data.animSheetName = "hulls/explosion1.acfg";
	data.layer = GraphicsLayer::ShipAppendagesLower;
	explosionAnim = sptr<GraphicsComponent>(new QuadComponent(data));
	explosionAnim->setPosition(m_fix.getCenter());
	explosionAnim->setRotation(m_fix.getAngle());

	//Evan - load 'explosion' sprite/animation
	data = QuadComponentData();
	data.dimensions.x = 256;
	data.dimensions.y = 256;
	data.permanentRot = 0;
	data.center.x = 0;
	data.center.y = 0;
	data.texName = "hulls/explosion1.png";
	data.animSheetName = "hulls/explosion1.acfg";
	data.layer = GraphicsLayer::ShipAppendagesLower;
	explosionAnimSmall = sptr<GraphicsComponent>(new QuadComponent(data));
	explosionAnimSmall->setPosition(m_fix.getCenter());
	explosionAnimSmall->setRotation(m_fix.getAngle());
	
	/*
	hit_buffer.loadFromFile("audio/hit1.wav");
	hit_sound.setBuffer(hit_buffer);
	hit_sound.setLoop(false);
	hit_sound.setVolume(100);
	explode_buffer1.loadFromFile("audio/explode_small1.wav");
	explode_sound1.setBuffer(explode_buffer1);
	explode_sound1.setLoop(false);
	explode_sound1.setVolume(100);
	explode_buffer2.loadFromFile("audio/explode1.wav");
	explode_sound2.setBuffer(explode_buffer2);
	explode_sound2.setLoop(false);
	explode_sound2.setVolume(50);*/

	//rand()
	positions.push_back(b2Vec2(.25f, .25f));
	positions.push_back(b2Vec2(.25f, -.25f));
	positions.push_back(b2Vec2(-.25f, -.25f));
	positions.push_back(b2Vec2(-.25f, .25f));
	positions.push_back(b2Vec2(-.25f, -.25f));
	positions.push_back(b2Vec2(.25f, .25f));
	isExploding = false;
	timer1 = 150;
	timer2 = 20;
	originalPos = b2Vec2(explosionAnimSmall->getPosition());
}
ShipModule::~ShipModule()
{

}
void ShipModule::prePhysUpdate()
{

}
void ShipModule::postPhysUpdate()
{
	for (int i = 0; i < (signed)m_decors.size(); ++i)
	{
		m_decors[i]->setPosition(m_fix.getCenter());
		m_decors[i]->setRotation(m_fix.getAngle());
	}

	//Evan - hit anim and explode anim positions
	hitAnim->setPosition(m_fix.getCenter());
	hitAnim->setRotation(m_fix.getAngle());
	explosionAnim->setPosition(m_fix.getCenter());
	explosionAnim->setRotation(m_fix.getAngle());
	originalPos = m_fix.getCenter();
	explosionAnimSmall->setRotation(m_fix.getAngle());

	//Evan - isExploding animation
	if (isExploding)
	{
		//when to do explosion anim
		if (timer1 < 0) 
		{
			hitAnim->getAnimator().setAnimation("Default", 1.0f);
			explosionAnim->getAnimator().setAnimation("Explode", 1.0f);
			setHealthState(HealthState::Broken); //?
			isExploding = false;
			//explode_sound2.play();
			game.getSound().playSound("explode1.wav", 100, 20, 20, m_fix.getCenter(), true);
		}

		//when to do explosion anim
		if (timer2 < 0 && !positions.empty())
		{
			explosionAnimSmall->setPosition(b2Vec2(originalPos.x + positions.back().x, originalPos.y + positions.back().y));
			explosionAnimSmall->getAnimator().setAnimation("Explode", .5f);
			timer2 = 20;
			positions.pop_back();
			//explode_sound1.play();
			game.getSound().playSound("explode_small2.wav", 100, 20, 20, m_fix.getCenter(), true);
		}
		timer1--;
		timer2--;
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

	if (rCommand == "damage")
	{
		int val;
		rData >> val;
		m_health.damage(val);
		m_io.event(EventType::Health, m_health.getHealth(), voidPacket);
		if (m_health.isDead())
		{
			//Evan - explosion animation
			//for literal explosion, continually reset position
			isExploding = true;
			hitAnim->getAnimator().setAnimation("Explode", 1.5f);
		}
		else if (m_health.getHealthPercent() <= damagedPercent)//damaged percent
			setHealthState(HealthState::Damaged);

		//Evan - play 'hit' animation (red flashing box)
		if (!m_health.isDead())
		{
			hitAnim->getAnimator().setAnimation("Hit", .20f);
			//hit_sound.play(); //todo - better sound
		}

	}
	else if (rCommand == "heal")
	{
		int val;
		rData >> val;
		m_health.heal(val);
		m_io.event(EventType::Health, m_health.getHealth(), voidPacket);
		if (m_health.getHealthPercent() >= damagedPercent)
			setHealthState(HealthState::Nominal);
	}
	else
		Module::input(rCommand, rData);
}
bool ShipModule::functioning()//does this module still do its function
{
	if (m_healthState == HealthState::Nominal)
		return true;
	else if (m_healthState == HealthState::Broken)
		return false;
	else if (m_healthState == HealthState::Damaged)
	{
		if (m_functionsDamaged)
			return true;
		else
			return false;
	}
	cout << "\n" << FILELINE;
	return true;
}
void ShipModule::setHealthState(HealthState newState)
{
	///fsm needed?
	//nominal -> damaged
	//no effect

	//nominal -> broken
	//set colcat broken

	//damaged -> broken
	//set colcat broken

	//damaged -> nominal
	//set colcat normal

	//broken -> damaged
	//set colcat broken

	//broken -> nominal
	//set colcat normal

	//else no effect cause we didn't change state

	m_healthState = newState;

	if (newState == HealthState::Broken)
	{
		m_fix.setCategory(Category::ShipModuleBroke);
		m_decors[0]->setColor(sf::Color(255, 0, 0, 255));///makes the sprite red when destroyed
		///WE SHOULD SET ALL SPRITES TO BE DAMAGED STATE
	}


	setHealthStateHook(newState);
}
void ShipModule::setHealthStateHook(HealthState newState)
{

}
