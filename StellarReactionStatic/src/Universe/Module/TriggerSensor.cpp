#include "TriggerSensor.hpp"
#include "BodyComponent.hpp"
#include "FixtureComponent.hpp"


TriggerSensor::TriggerSensor(const TriggerSensorData& rData) : Sensor(rData)
{
	m_maxDamage = rData.max_dmg;
	m_minDamage = rData.min_dmg;
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
				damageAmount = (m_maxDamage / (sqLen));
			else
				damageAmount = m_maxDamage;
			if (damageAmount < m_minDamage)
				damageAmount = m_minDamage;
			sf::Packet packet;
			packet << damageAmount << this->getFixtureComponent().getIOPos();
			Message mess;
			mess.reset((*it)->getIOPos(), "damage", packet, 0.f, false);
			game.getUniverse().getUniverseIO().recieve(mess);
		}
		m_damageTimer.setCountDown(m_period);
		m_damageTimer.restartCountDown();
	}

}
void TriggerSensorData::loadJson(const Json::Value& root)
{
	if (!root["MaxDamage"].isNull())
		max_dmg = root["MaxDamage"].asFloat();
	if (!root["MinDamage"].isNull())
		min_dmg = root["MinDamage"].asFloat();
	if (!root["Period"].isNull())
		period = root["Period"].asFloat();
	SensorData::loadJson(root);
}
