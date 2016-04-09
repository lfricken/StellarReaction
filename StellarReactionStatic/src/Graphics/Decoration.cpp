#include "Decoration.hpp"

#include "GraphicsComponent.hpp"
#include "JSON.hpp"

//Evan
#include "Player.hpp"
#include "Globals.hpp"
#include "Camera.hpp"
#include "Convert.hpp"

using namespace std;

void DecorationData::loadJson(const Json::Value& root)
{
	LOADJSON(ioComp);
	GETJSON(movementScale);
	GETJSON(isAbsoluteSize);
	GETJSON(initPosition);
	GETJSON(velocity);
	GETJSON(repeats);
}

Decoration::Decoration(const DecorationData& rData, GraphicsComponent* pGfx) : m_io(rData.ioComp, &Decoration::input, this)
{
	m_gfx = pGfx;
	m_initPosition = rData.initPosition;
	setPosition(rData.initPosition);
	m_movementScale = rData.movementScale;

	//Evan
	velocity = rData.velocity;
	velocityTimer.getTimeElapsed();
	num_in_layer = b2Vec2(rData.num_in_layer);
	m_repeats = rData.repeats;
}
Decoration::~Decoration()
{

}
bool Decoration::hasAbsoluteSize() const
{
	return m_isAbsoluteSize;
}
void Decoration::input(std::string rCommand, sf::Packet rData)
{
	if(!inputHook(rCommand, rData))
	{
		if(rCommand == "setPosition")
		{
			b2Vec2 pos;
			rData >> pos.x;
			rData >> pos.y;
			setPosition(pos);
		}
		else if(rCommand == "setRotation")
		{
			float rotCCW;
			rData >> rotCCW;
			setRotation(rotCCW);
		}
		else if(rCommand == "setAnimation")
		{
			string anim;
			float duration;
			rData >> anim;
			rData >> duration;
			m_gfx->getAnimator().setAnimation(anim, duration);
		}
		else
		{
			cout << "\n[" << rCommand << "] was not found in [" << m_io.getName() << "].";
			///ERROR LOG
		}
	}
}
void Decoration::setPosition(const b2Vec2& rWorld)
{
	m_position = rWorld;
	m_gfx->setPosition(m_position);
}
void Decoration::setRotation(float radiansCCW)
{
	m_rotation = radiansCCW;
	m_gfx->setRotation(m_rotation);
}
void Decoration::setAnimation(const std::string& rAnimName, float duration)
{
	m_gfx->getAnimator().setAnimation(rAnimName, duration);
}
void Decoration::setScale(float scale)
{
	m_gfx->setScale(scale);
}
void Decoration::updateScaledPosition(const b2Vec2& rCameraCenter)
{

	if(m_movementScale == 0.0f)
	{
		setPosition(m_initPosition);
		return;
	}

	//Evan - update velocity over time
	float time = velocityTimer.getTimeElapsed();
	b2Vec2 deltaV(velocity.x * time / m_movementScale * sizeScalingFactor, velocity.y * time / m_movementScale * sizeScalingFactor);
	m_initPosition += deltaV;

	if(m_repeats)
	{
		int max_x = 80 * sizeScalingFactor;
		int max_y = 60 * sizeScalingFactor;


		//this logic should be in another class, not the default decoration
		//below lines are magic
		if(m_initPosition.x + rCameraCenter.x * m_movementScale > (rCameraCenter.x) + (max_x))
		{
			m_initPosition.x -= (dimensions.x / scale)*num_in_layer.x;
		}
		else if(m_initPosition.x + rCameraCenter.x * m_movementScale < (rCameraCenter.x) - (max_x))
		{
			m_initPosition.x += (dimensions.x / scale)*num_in_layer.x;
		}
		if(m_initPosition.y + rCameraCenter.y * m_movementScale >(rCameraCenter.y + (max_y)))
		{
			m_initPosition.y -= (dimensions.y / scale)*num_in_layer.y;
		}
		else if(m_initPosition.y + rCameraCenter.y * m_movementScale < (rCameraCenter.y - (max_y)))
		{
			m_initPosition.y += (dimensions.y / scale)*num_in_layer.y;
		}
	}

	setPosition(m_initPosition + b2Vec2(rCameraCenter.x*m_movementScale, rCameraCenter.y*m_movementScale));
}