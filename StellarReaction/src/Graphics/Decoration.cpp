#include "Decoration.hpp"

#include "GraphicsComponent.hpp"

using namespace std;

Decoration::Decoration(const DecorationData& rData, GraphicsComponent* pGfx) : m_io(rData.ioComp, &Decoration::input, this)
{
	m_gfx = pGfx;
	m_initPosition = rData.initPosition;
	setPosition(rData.initPosition);
	m_movementScale = rData.movementScale;
	m_isAbsoluteSize = rData.isAbsoluteSize;
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
	setPosition(m_initPosition + b2Vec2(rCameraCenter.x*m_movementScale, rCameraCenter.y*m_movementScale));
}