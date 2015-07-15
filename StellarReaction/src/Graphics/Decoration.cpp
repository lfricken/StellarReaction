#include "Decoration.hpp"

#include "GraphicsComponent.hpp"

using namespace std;

Decoration::Decoration(const DecorationData& rData, GraphicsComponent* pGfx) : m_io(rData.ioComp, &Decoration::input, this)
{
	m_gfx = pGfx;
}
Decoration::~Decoration()
{

}
void Decoration::input(std::string rCommand, sf::Packet rData)
{
	if(rCommand == "setPosition")
	{
		b2Vec2 pos;
		rData >> pos.x;
		rData >> pos.y;
		setPosition(pos);
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
