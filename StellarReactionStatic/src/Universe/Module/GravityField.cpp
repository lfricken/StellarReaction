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
	m_radius = rData.fixComp.size.x;
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
		BodyComponent* pBody = (**it).getParentBody();
		//float bodyMass = pBody->GetMass();

		Vec2 targetPos = pBody->getPosition();
		Vec2 myPos = m_fix.getCenter();
		Vec2 direction = myPos - targetPos;

		float radiusFraction = 1 - (direction.len() / m_radius);

		float force = radiusFraction * m_maxForce;
		if(force < m_minForce)
			force = m_minForce;

		direction = direction.unit() * 100;
		//direction.x *= force*bodyMass;
		//direction.y *= force*bodyMass;
		pBody->applyForce(direction);
	}

}
