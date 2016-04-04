#include "GravityField.hpp"
#include "BodyComponent.hpp"
#include "FixtureComponent.hpp"


void GravityFieldData::loadJson(const Json::Value& root)
{
	GETJSON(maxForce);
	GETJSON(minForce);
	SensorData::loadJson(root);
}
GravityField::GravityField(const GravityFieldData& rData) : Sensor(rData)
{
	m_radius = rData.fixComp.size.x / 2.f * sizeScalingFactor;
	m_maxForce = rData.maxForce;
	m_minForce = rData.minForce;
}
GravityField::~GravityField()
{

}
void GravityField::prePhysUpdate()
{
	for(auto it = m_guests.cbegin(); it != m_guests.cend(); ++it)
	{
		b2Body* pBody = (**it).getBodyPtr();
		float bodyMass = pBody->GetMass();

		b2Vec2 targetPos = pBody->GetPosition();
		b2Vec2 myPos = m_fix.getCenter();
		b2Vec2 direction = myPos - targetPos;

		float radiusFraction = 1 - (direction.Length() / m_radius);

		float force = radiusFraction * m_maxForce;
		if(force < m_minForce)
			force = m_minForce;

		direction.Normalize();
		direction.x *= force*bodyMass;
		direction.y *= force*bodyMass;
		pBody->ApplyForceToCenter(direction, true);
	}

}
