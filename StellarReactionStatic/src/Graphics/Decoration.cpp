#include "Decoration.hpp"
#include "GraphicsComponent.hpp"
#include "JSON.hpp"
#include "Random.hpp"
#include "Convert.hpp"
#include "Player.hpp"

void DecorationData::loadJson(const Json::Value& root)
{
	LOADJSON(ioComp);

	GETJSON(minSpinRate);
	GETJSON(maxSpinRate);
	GETJSON(minVelocity);
	GETJSON(maxVelocity);

	GETJSON(realPosition);
	GETJSON(zPos);

	GETJSON(infiniteZ);
	GETJSON(repeats);
	GETJSON(repeatsRandom);
	GETJSON(spawnRandom);
}
Decoration::Decoration(const DecorationData& data, GraphicsComponent* pGfx) : m_io(data.ioComp, &Decoration::input, this)
{
	m_spGfx.reset(pGfx);

	m_minSpin = leon::degToRad(data.minSpinRate);
	m_maxSpin = leon::degToRad(data.maxSpinRate);
	m_minVel = data.minVelocity;
	m_maxVel = data.maxVelocity;

	m_realPosition = data.realPosition;
	m_zPos = data.zPos;

	m_infiniteZ = data.infiniteZ;
	m_repeats = data.repeats;
	m_repeatsRandom = data.repeatsRandom;
	m_spawnRandom = data.spawnRandom;


	m_lastCameraPos = Vec2(0, 0);
	m_maxZoom = game.getLocalPlayer().getCamera().m_maxZoom;
	randSpin();
	randVel();
}
Decoration::~Decoration()
{

}
void Decoration::randVel()
{
	m_spinRate = Random::get(m_minSpin, m_maxSpin);
}
void Decoration::randSpin()
{
	m_velocity.x = Random::get(m_minVel.x, m_maxVel.x);
	m_velocity.y = Random::get(m_minVel.y, m_maxVel.y);
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
	{
		float x = rDesiredWorldPos.x - m_lastCameraPos.x;//x dist from camera
		const float slope = x / m_maxZoom;//slope
		const float zDist = m_maxZoom + m_zPos;//z dist from camera to source
		m_realPosition.x = zDist*slope + rDesiredWorldPos.x;//xPos
	}
	{
		float y = rDesiredWorldPos.y - m_lastCameraPos.y;//y dist from camera
		const float slope = y / m_maxZoom;//slope
		const float zDist = m_maxZoom + m_zPos;//z dist from camera to source
		m_realPosition.y = zDist*slope + rDesiredWorldPos.y;//yPos
	}
	randVel();
	randSpin();
}
void Decoration::setRotation(float radCCW)
{
	m_spGfx->setRotation(radCCW);
}
void Decoration::setAnimation(const std::string& rAnimName, float duration)
{
	m_spGfx->getAnimator().setAnimation(rAnimName, duration);
}
void Decoration::setScale(float scale)
{
	m_spGfx->setScale(scale);
}
bool Decoration::isRandSpawn() const
{
	return m_spawnRandom;
}
void Decoration::updateScaledPosition(const Vec2& rCameraCenter, const Vec2& bottomLeft, const Vec2& topRight, const float zoom, const float dTime)
{
	m_lastCameraPos = rCameraCenter;
	const float zDist = zoom + m_zPos;//z dist from camera to source
	const Vec2 halfSize(m_spGfx->getSize().x / 2.f, m_spGfx->getSize().y / 2.f);

	/**Spins**/
	{
		const float total = m_spGfx->getRotation() + (dTime * m_spinRate);
		m_spGfx->setRotation(total);
	}

	/**Follows Camera**/
	{
		m_realPosition += Vec2(m_velocity.x * dTime, m_velocity.y * dTime);
		Vec2 appearPos(0, 0);
		if(!m_infiniteZ)
		{
			const float zoom = game.getLocalPlayer().getCamera().getZoom();
			const float xDist = m_realPosition.x - rCameraCenter.x;
			const float yDist = m_realPosition.y - rCameraCenter.y;
			{
				const float slope = xDist / zDist;
				appearPos.x = slope*zoom;
			}
			{
				const float slope = yDist / zDist;
				appearPos.y = slope*zoom;
			}
		}
		else
			appearPos = m_realPosition;

		m_spGfx->setPosition(appearPos + rCameraCenter);
	}


	/**Repeats itself over view box**/
	/**Relies on graphics object so position is by appearance**/
	{
		const Vec2 pos(m_spGfx->getPosition());
		const Vec2 ourBotLeft = pos - halfSize;
		const Vec2 ourTopRight = pos + halfSize;

		if(m_repeatsRandom || m_repeats)
		{
			if(ourTopRight.x < bottomLeft.x)//has exited left
			{
				float starting = m_spGfx->getPosition().y;

				if(m_repeatsRandom)
					starting = Random::get(bottomLeft.y, topRight.y);

				setPosition(Vec2(topRight.x, starting));
			}
			if(ourBotLeft.x > topRight.x)//has exited right
			{
				float starting = m_spGfx->getPosition().y;

				if(m_repeatsRandom)
					starting = Random::get(bottomLeft.y, topRight.y);

				setPosition(Vec2(bottomLeft.x, starting));
			}
			if(ourBotLeft.y > topRight.y)//has exited top
			{
				float starting = m_spGfx->getPosition().x;

				if(m_repeatsRandom)
					starting = Random::get(bottomLeft.x, topRight.x);

				setPosition(Vec2(starting, bottomLeft.y));
			}
			if(ourTopRight.y < bottomLeft.y)//has exited bottom
			{
				float starting = m_spGfx->getPosition().x;

				if(m_repeatsRandom)
					starting = Random::get(bottomLeft.x, topRight.x);

				setPosition(Vec2(starting, topRight.y));
			}
		}
	}


	/**Scales with zoom out**/
	const float staticSize = (1 * (zoom - 1) + 1);//counters the zoom scaling
	if(m_infiniteZ)
	{
		setScale(staticSize);
	}
	else
	{
		const float staticZoom = (m_maxZoom + m_zPos);
		const float angle = atan(halfSize.x / zDist);
		setScale(angle*staticSize*staticZoom);
	}
}

