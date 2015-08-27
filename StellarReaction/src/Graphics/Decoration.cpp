#include "Decoration.hpp"

#include "GraphicsComponent.hpp"

//Evan
#include "Player.hpp"
#include "Globals.hpp"
#include "Camera.hpp"

using namespace std;

Decoration::Decoration(const DecorationData& rData, GraphicsComponent* pGfx) : m_io(rData.ioComp, &Decoration::input, this)
{
	m_gfx = pGfx;
	m_initPosition = rData.initPosition;
	setPosition(rData.initPosition);
	m_movementScale = rData.movementScale;

	velocity = b2Vec2(.25f, .25f);
	velocityTimer.getTimeElapsed();
	dimensions = rData.dimensions;
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
	int max_x = 600; //got manually via cout
	int max_y = 16800;
	//cout << endl << "cam y: " << game.getLocalPlayer().getCamera().getView().getSize().y;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
		//cout << endl << "m_initPosition.x: " << m_initPosition.x;
		cout << endl << "rCameraCenter.y: " << rCameraCenter.y;
	}

	if (m_initPosition.x > (rCameraCenter.x + (max_x / 2)))
	{
		m_initPosition.x -= (dimensions.x / scale)*;
		cout << endl << "jump to: " << m_initPosition.x;
	}	
	//else if (m_initPosition.x < (rCameraCenter.x - (max_x / 2)))
	//{
	//	m_initPosition.x += dimensions.x / scale;
	//	cout << endl << "jump to: " << m_initPosition.x;
	//}
	//if (m_initPosition.y > (rCameraCenter.y - (max_y / 2)))
	//{
	//	m_initPosition.y -= max_y;
	//}
	//else if (m_initPosition.y < (rCameraCenter.y - (max_y / 2)))
	//{
	//	m_initPosition.y += max_y;
	//}


	setPosition(m_initPosition + b2Vec2(rCameraCenter.x*m_movementScale, rCameraCenter.y*m_movementScale));
}