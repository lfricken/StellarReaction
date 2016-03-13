#include "TriggerSensor.hpp"
#include "BodyComponent.hpp"
#include "FixtureComponent.hpp"


TriggerSensor::TriggerSensor(const TriggerSensorData& rData) : Sensor(rData)
{
	m_damage = rData.dmg;
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
			float damageAmount = (m_damage / (sqLen))*game.getUniverse().getTimeStep();
			damageAmount = 1.f;
			sf::Packet packet;
			packet << damageAmount << (*it)->getIOPos();
			Message mess;
			mess.reset((*it)->getIOPos(), "damage", packet, 0.f, false);
			game.getUniverse().getUniverseIO().recieve(mess);
		}
		m_damageTimer.setCountDown(5);
		m_damageTimer.restartCountDown();
	}

}
void TriggerSensorData::loadJson(const Json::Value& root)
{
	if (!root["Damage"].isNull())
		dmg = root["Damage"].asFloat();
	SensorData::loadJson(root);
}
