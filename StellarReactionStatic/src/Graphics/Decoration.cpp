#include "Decoration.hpp"
#include "GraphicsComponent.hpp"
#include "JSON.hpp"
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
Decoration::Decoration(const DecorationData& data, sptr<GraphicsComponent> pGfx)// : m_io(data.ioComp, &Decoration::input, this)
{
	m_spGfx = pGfx;

	m_minSpin = Math::toRad(data.minSpinRate);
	m_maxSpin = Math::toRad(data.maxSpinRate);
	m_minVel = data.minVelocity;
	m_maxVel = data.maxVelocity;

	m_realPosition = data.realPosition;
	m_zPos = data.zPos;

	m_infiniteZ = data.infiniteZ;
	m_repeats = data.repeats;
	m_repeatsRandom = data.repeatsRandom;
	m_spawnRandom = data.spawnRandom;


	m_lastCameraPos = Vec2(0, 0);
	m_maxZoom = getGame()->getLocalPlayer().getCamera().m_maxZoom;
	randSpin();
	randVel();

	m_isFadingOut = false;
	m_totalFadeOutTime = 1;
	m_fadeOutTimeElapsed = 0;


	m_isFadingIn = false;
	m_totalFadeInTime = 1;
	m_fadeInTimeElapsed = 0;


}
Decoration::~Decoration()
{

}
void Decoration::randVel()
{
	m_velocity.x = Rand::get(m_minVel.x, m_maxVel.x);
	m_velocity.y = Rand::get(m_minVel.y, m_maxVel.y);
}
void Decoration::randSpin()
{
	m_spinRate = Rand::get(m_minSpin, m_maxSpin);
	if(m_minSpin != 0.f)
		m_spGfx->setRotation(Rand::get(0.f, Math::Tau));
}
//void Decoration::input(String rCommand, sf::Packet rData)
//{
//	if(rCommand == "setPosition")
//	{
//		Vec2 pos;
//		rData >> pos.x;
//		rData >> pos.y;
//		setPosition(pos);
//	}
//	else if(rCommand == "setRotation")
//	{
//		float rotCCW;
//		rData >> rotCCW;
//		setRotation(rotCCW);
//	}
//	else if(rCommand == "setAnimation")
//	{
//		String anim;
//		float duration;
//		rData >> anim;
//		rData >> duration;
//		m_spGfx->getAnimator().setAnimation(anim, duration);
//	}
//	else
//	{
//		Print << "\n[" << rCommand << "] was not found in [" << m_io.getName() << "].";
//		///ERROR LOG
//	}
//}
void Decoration::setPosition(const Vec2& rDesiredWorldPos)
{
	float zoom = getGame()->getLocalPlayer().getCamera().getZoom();
	{
		float x = rDesiredWorldPos.x - m_lastCameraPos.x;//x dist from camera
		const float slope = x / m_maxZoom;//slope
		const float zDist = m_maxZoom + m_zPos;//z dist from camera to source
		m_realPosition.x = zDist*slope + m_lastCameraPos.x;//xPos
	}
	{
		float y = rDesiredWorldPos.y - m_lastCameraPos.y;//y dist from camera
		const float slope = y / m_maxZoom;//slope
		const float zDist = m_maxZoom + m_zPos;//z dist from camera to source
		m_realPosition.y = zDist*slope + m_lastCameraPos.y;//yPos
	}
	randVel();
	randSpin();
}
void Decoration::setRotation(float radCCW)
{
	m_spGfx->setRotation(radCCW);
}
void Decoration::setAnimation(const String& rAnimName, float duration)
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
void Decoration::updateScaledPosition(const Vec2& rCameraCenter, const Vec2& bottomLeft2, const Vec2& topRight2, const float zoom, const float dTime)
{
	const Vec2 shrink(3, 3);
	const Vec2 bottomLeft = bottomLeft2 - shrink;
	const Vec2 topRight = topRight2 - shrink;

	m_lastCameraPos = rCameraCenter;
	const float zDist = zoom + m_zPos;//z dist from camera to source
	const Vec2 halfSize(m_spGfx->getUnscaledSize().x / 2.f, m_spGfx->getUnscaledSize().y / 2.f);

	/**Spins**/
	{
		const float total = m_spGfx->getRotation() + (dTime * m_spinRate);
		m_spGfx->setRotation(total);
	}

	/**Follows Camera**/
	{
		m_realPosition += m_velocity*dTime;
		Vec2 appearPos(0, 0);
		if(!m_infiniteZ)
		{
			const float zoom = getGame()->getLocalPlayer().getCamera().getZoom();
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
		const Vec2 pos = m_spGfx->getPosition();
		const Vec2 ourBotLeft = pos - halfSize;
		const Vec2 ourTopRight = pos + halfSize;

		if(m_repeatsRandom || m_repeats)
		{
			if(ourTopRight.x < bottomLeft.x) // has exited left
			{
				float starting = m_spGfx->getPosition().y;

				if(m_repeatsRandom)
					starting = Rand::get(bottomLeft.y, topRight.y);

				setPosition(Vec2(topRight.x, starting));
			}
			if(ourBotLeft.x > topRight.x) // has exited right
			{
				float starting = m_spGfx->getPosition().y;

				if(m_repeatsRandom)
					starting = Rand::get(bottomLeft.y, topRight.y);

				setPosition(Vec2(bottomLeft.x, starting));
			}
			if(ourBotLeft.y > topRight.y) // has exited top
			{
				float starting = m_spGfx->getPosition().x;

				if(m_repeatsRandom)
					starting = Rand::get(bottomLeft.x, topRight.x);

				setPosition(Vec2(starting, bottomLeft.y));
			}
			if(ourTopRight.y < bottomLeft.y) // has exited bottom
			{
				float starting = m_spGfx->getPosition().x;

				if(m_repeatsRandom)
					starting = Rand::get(bottomLeft.x, topRight.x);

				setPosition(Vec2(starting, topRight.y));
			}
		}
	}


	// scales with zoom out
	const float staticSize = zoom; // counters the normal zoom scaling
	if(m_infiniteZ)
	{
		setScale(staticSize);
	}
	else
	{
		const float one_scale_zoom = 1; // at what zoom should the scale of the sprite be 1?
		const float staticZoom = (one_scale_zoom + m_zPos);
		// divide by half size otherwise you square size because 
		// you factor size into scale, which factors into size
		const float relativeAngle = atan(halfSize.x / zDist) / halfSize.x;
		setScale(relativeAngle * staticSize * staticZoom);
	}

	// compute fading in
	if(m_isFadingIn)
	{
		if(m_fadeInTimeElapsed <= m_totalFadeInTime)
		{
			m_fadeInTimeElapsed += dTime;//if we dont take min of 1,x then it can go past 255 alpha and reappear for a second
			int alpha = (int)(0.f + Math::min(1.f, (m_fadeInTimeElapsed / m_totalFadeInTime)) * 255.f);
			m_spGfx->setAlpha(alpha);
		}
		else // fade time expired
		{
			m_isFadingIn = false;
			m_spGfx->setAlpha(255);
		}
	}
	// compute fading out
	if(m_isFadingOut)
	{
		if(m_fadeOutTimeElapsed <= m_totalFadeOutTime)
		{
			m_fadeOutTimeElapsed += dTime;// if we dont take min of 1,x then it can go past 255 alpha and reappear for a second
			// TODO make it so it can fade from non max alpha (so fade in partial, fade out partial)
			int alpha = (int)(255.f - Math::min(1.f, (m_fadeOutTimeElapsed / m_totalFadeOutTime)) * 255.f);
			m_spGfx->setAlpha(alpha);
		}
		else // fade time expired
		{
			m_spGfx->setAlpha(0);
		}
	}
}
void Decoration::startFadeOut(float time)
{
	m_isFadingOut = true;
	m_totalFadeOutTime = time;
	m_fadeOutTimeElapsed = 0;
}
void Decoration::startFadeIn(float time)
{
	m_totalFadeInTime = time;
	m_fadeInTimeElapsed = 0;
	if(m_totalFadeInTime > 0)
	{
		m_isFadingIn = true;
		m_spGfx->setAlpha(0);
	}
}

