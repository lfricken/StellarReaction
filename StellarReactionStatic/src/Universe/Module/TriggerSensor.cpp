#include "TriggerSensor.hpp"
#include "BodyComponent.hpp"
#include "FixtureComponent.hpp"
#include "Weapon.hpp"

void TriggerSensorData::loadJson(const Json::Value& root)
{
	GETJSON(maxDamage);
	GETJSON(minDamage);
	GETJSON(period);
	SensorData::loadJson(root);
}
TriggerSensor::TriggerSensor(const TriggerSensorData& rData) : Sensor(rData)
{
	m_maxDamage = rData.maxDamage;
	m_minDamage = rData.minDamage;
	m_period = rData.period;
	m_radius = rData.fixComp.size.x / 2.f * sizeScalingFactor;
}
TriggerSensor::~TriggerSensor()
{

}
void TriggerSensor::prePhysUpdate()
{
	if(m_damageTimer.isTimeUp())
	{
		for(auto it = m_guests.cbegin(); it != m_guests.cend(); ++it)
		{
			b2Body* bod = (*it)->getBodyPtr();
			Vec2 myPos = m_fix.getCenter();
			Vec2 targetPos = bod->GetPosition();
			Vec2 direction = myPos - targetPos;

			float fractionToCenter = 1 - (direction.len() / m_radius);

			int damage = (int)(fractionToCenter * m_maxDamage);
			if(damage < m_minDamage)
				damage = m_minDamage;

			Weapon::damage(&game.getUniverse().getUniverseIO(), (*it)->getIOPos(), damage, getFixtureComponent().getIOPos(), m_parentChunk->getBodyComponent().getTeam(), Vec2(0, 0), Vec2(0, 0), "");
		}
		m_damageTimer.setCountDown(m_period);
		m_damageTimer.restartCountDown();
	}

}

