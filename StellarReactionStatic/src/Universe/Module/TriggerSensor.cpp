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
}
TriggerSensor::~TriggerSensor()
{

}
void TriggerSensor::prePhysUpdate()
{
	if (m_damageTimer.isTimeUp()){
		for (auto it = m_guests.cbegin(); it != m_guests.cend(); ++it)
		{
			b2Body* bod = (*it)->getBodyPtr();
			b2Vec2 myPos = m_parentChunk->getBodyPtr()->GetPosition();
			b2Vec2 targetPos = bod->GetPosition();

			b2Vec2 direction = myPos - targetPos;
			float sqLen = direction.LengthSquared();
			int damageAmount;
			if (sqLen > 4)
				damageAmount = static_cast<int>((m_maxDamage / (sqLen)));
			else
				damageAmount = m_maxDamage;
			if (damageAmount < m_minDamage)
				damageAmount = m_minDamage;

			Weapon::damage(&game.getUniverse().getUniverseIO(), (*it)->getIOPos(), damageAmount, getFixtureComponent().getIOPos());
		}
		m_damageTimer.setCountDown(m_period);
		m_damageTimer.restartCountDown();
	}

}

