#include "GravityField.hpp"
#include "BodyComponent.hpp"
#include "FixtureComponent.hpp"


GravityField::GravityField(const GravityFieldData& rData) : Sensor(rData)
{
	m_mass = rData.mass;
}
GravityField::~GravityField()
{

}
void GravityField::prePhysUpdate()
{
	for (auto it = m_guests.cbegin(); it != m_guests.cend(); ++it)
	{
		b2Body* bod = (*it)->getBodyPtr();
		b2Vec2 myPos = m_parentChunk->getBodyPtr()->GetPosition();
		b2Vec2 targetPos = bod->GetPosition();

		b2Vec2 direction = myPos - targetPos;
		float sqLen = direction.LengthSquared();
		direction.Normalize();
		if ( (sqLen > 2)) {
			direction *= m_mass*bod->GetMass() / (sqLen);
			bod->ApplyForceToCenter(direction, true);
		}
		else {
			direction *= 50;
			bod->ApplyForceToCenter(direction, true);
		}
	}

}
void GravityFieldData::loadJson(const Json::Value& root)
{
	if (!root["Mass"].isNull())
		mass = root["Mass"].asFloat();
	SensorData::loadJson(root);
}
