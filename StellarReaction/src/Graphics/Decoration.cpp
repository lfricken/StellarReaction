#include "Decoration.hpp"

#include "GraphicsComponent.hpp"

//Evan
#include "Player.hpp"
#include "Globals.hpp"
#include "Camera.hpp"
#include "Convert.hpp"

using namespace std;

Decoration::Decoration(const DecorationData& rData, GraphicsComponent* pGfx) : m_io(rData.ioComp, &Decoration::input, this)
{
	m_gfx = pGfx;
	m_initPosition = rData.initPosition;
	setPosition(rData.initPosition);
	m_movementScale = rData.movementScale;

	//Evan
	velocity = b2Vec2(.25f, .25f);
	velocityTimer.getTimeElapsed();
	dimensions = b2Vec2(rData.dimensions);
	num_in_layer = b2Vec2(rData.num_in_layer);

}
Decoration::~Decoration()
{

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
void Decoration::updateScaledPosition(const b2Vec2& rCameraCenter)
{
	//Evan - update velocity over time
	float time = velocityTimer.getTimeElapsed();
	b2Vec2 deltaV(velocity.x*time/m_movementScale, velocity.y*time/m_movementScale);
	m_initPosition += deltaV;

	//Evan - check for wrap around
	//int max_x = 33600; //got manually via cout
	//int max_y = 16800;
	int max_x = 80; //got manually via cout
	int max_y = 60;
	//cout << endl << "cam y: " << game.getLocalPlayer().getCamera().getView().getSize().y;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
		//cout << endl << "m_initPosition.x: " << m_initPosition.x;
	}

	//below lines are magic
	if (m_initPosition.x + rCameraCenter.x * m_movementScale > (rCameraCenter.x) + (max_x))
	{
		//cout << endl << "jump from: " << m_initPosition.x;
		//cout << endl << "jump amt: " << -(dimensions.x / scale)*num_in_layer.x;
		//cout << endl << "camera x : " << rCameraCenter.x;
		//system("PAUSE");
		m_initPosition.x -= (dimensions.x / scale)*num_in_layer.x;
	}	
	else if (m_initPosition.x + rCameraCenter.x * m_movementScale < (rCameraCenter.x) - (max_x))
	{
		m_initPosition.x += (dimensions.x / scale)*num_in_layer.x;
	}
	if (m_initPosition.y + rCameraCenter.y * m_movementScale > (rCameraCenter.y + (max_y)))
	{
		m_initPosition.y -= (dimensions.y / scale)*num_in_layer.y;
	}
	else if (m_initPosition.y + rCameraCenter.y * m_movementScale< (rCameraCenter.y - (max_y)))
	{
		m_initPosition.y += (dimensions.y / scale)*num_in_layer.y;
	}


	setPosition(m_initPosition + b2Vec2(rCameraCenter.x*m_movementScale, rCameraCenter.y*m_movementScale));
}