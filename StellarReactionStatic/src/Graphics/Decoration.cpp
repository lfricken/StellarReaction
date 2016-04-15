#include "Decoration.hpp"
#include "GraphicsComponent.hpp"
#include "JSON.hpp"

void DecorationData::loadJson(const Json::Value& root)
{
	LOADJSON(ioComp);
	GETJSON(movementScale);
	GETJSON(realPosition);

	GETJSON(minVelocity);
	GETJSON(maxVelocity);
	GETJSON(tiled);
}
Decoration::Decoration(const DecorationData& rData, GraphicsComponent* pGfx) : m_io(rData.ioComp, &Decoration::input, this)
{
	m_spGfx.reset(pGfx);

	setPosition(rData.realPosition);
	m_movementScale = rData.movementScale;
	m_velocity = rData.maxVelocity;
	m_realPosition = rData.realPosition;

	m_lastCameraPos = Vec2(0, 0);
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
		m_spGfx->getAnimator().setAnimation(anim, duration);
	}
	else
	{
		std::cout << "\n[" << rCommand << "] was not found in [" << m_io.getName() << "].";
		///ERROR LOG
	}
}
void Decoration::setPosition(const Vec2& rDesiredWorldPos)
{
	m_realPosition = rDesiredWorldPos - Vec2(m_lastCameraPos.x*m_movementScale, m_lastCameraPos.y*m_movementScale);
}
void Decoration::setRotation(float radiansCCW)
{
	m_spGfx->setRotation(radiansCCW);
}
void Decoration::setAnimation(const std::string& rAnimName, float duration)
{
	m_spGfx->getAnimator().setAnimation(rAnimName, duration);
}
void Decoration::setScale(float scale)
{
	m_spGfx->setScale(scale);
}
void Decoration::updateScaledPosition(const Vec2& rCameraCenter, const Vec2& bottomLeft, const Vec2& topRight, float dTime)
{
	m_lastCameraPos = rCameraCenter;
	m_realPosition += Vec2(m_velocity.x * dTime * (1.f - m_movementScale), m_velocity.y * dTime * (1.f - m_movementScale));
	m_spGfx->setPosition(m_realPosition + Vec2(rCameraCenter.x*m_movementScale, rCameraCenter.y*m_movementScale));

	Vec2 halfSize(m_spGfx->getSize().x / 2.f, m_spGfx->getSize().y / 2.f);
	Vec2 pos(m_spGfx->getPosition());
	Vec2 ourBotLeft = pos - halfSize;
	Vec2 ourTopRight = pos + halfSize;

	if(ourTopRight.x < bottomLeft.x)//has exited left
		setPosition(Vec2(topRight.x, m_spGfx->getPosition().y));

	if(ourBotLeft.x > topRight.x)//has exited right
		setPosition(Vec2(bottomLeft.x, m_spGfx->getPosition().y));

	if(ourBotLeft.y > topRight.y)//has exited top
		setPosition(Vec2(m_spGfx->getPosition().x, bottomLeft.y));

	if(ourTopRight.y < bottomLeft.y)//has exited bottom
		setPosition(Vec2(m_spGfx->getPosition().x, topRight.y));

}

