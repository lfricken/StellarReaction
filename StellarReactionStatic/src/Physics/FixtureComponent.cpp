#include "FixtureComponent.hpp"
#include "Globals.hpp"
#include "Convert.hpp"
#include "Universe.hpp"
#include "JSON.hpp"

using namespace std;


void FixtureComponentData::loadJson(const Json::Value& root)
{
	if(!root["shape"].isNull())
	{
		String temp = root["shape"].asString();
		if(temp == "rectangle")
			shape = leon::Shape::Rectangle;
		else if(temp == "circle")
			shape = leon::Shape::Circle;
		else
		{
			cout << "\n" << FILELINE;
			shape = leon::Shape::Circle;
		}
	}

	GETJSON(offset);
	GETJSON(size);
	GETJSON(density);
	GETJSON(friction);
	GETJSON(restitution);
	GETJSON(isSensor);

	if(!root["colCat"].isNull())
		colCategory = ChooseCategory(root["colCat"].asString());
	if(!root["colMask"].isNull())
		colMask = ChooseMask(root["colMask"].asString());
}
FixtureComponent::FixtureComponent(const FixtureComponentData& rData)
{
	m_offset = rData.offset;
	m_ioPos = -1;
	/**RECTANGLE**/
	if(rData.shape == leon::Shape::Rectangle)
	{
		m_spShape = sptr<b2Shape>(new b2PolygonShape);
		Vec2 offset(m_offset.x * sizeScalingFactor, m_offset.y * sizeScalingFactor);
		static_cast<b2PolygonShape*>(m_spShape.get())->SetAsBox(rData.size.x / 2.f * sizeScalingFactor, rData.size.y / 2.f * sizeScalingFactor, offset, 0);
	}
	/**CIRCLE**/
	else if(rData.shape == leon::Shape::Circle)
	{
		m_spShape = sptr<b2Shape>(new b2CircleShape);
		b2CircleShape* temp = static_cast<b2CircleShape*>(m_spShape.get());
		temp->m_p.Set(m_offset.x * sizeScalingFactor, m_offset.y * sizeScalingFactor);
		temp->m_radius = rData.size.x / 2.f * sizeScalingFactor;
	}


	m_fixtureDef.isSensor = rData.isSensor;
	m_fixtureDef.shape = &*m_spShape;//give our shape to our fixture definition
	m_fixtureDef.density = rData.density;
	m_fixtureDef.friction = rData.friction;
	m_fixtureDef.restitution = rData.restitution;//setting our fixture data
	m_fixtureDef.filter.maskBits = static_cast<uint16_t>(rData.colMask);
	m_fixtureDef.filter.categoryBits = static_cast<uint16_t>(rData.colCategory);

	if(rData.pBody != NULL)
	{
		m_pFixture = rData.pBody->CreateFixture(&m_fixtureDef);
		m_pFixture->SetUserData(this);
	}
	else
		cout << FILELINE;
}
FixtureComponent::~FixtureComponent()
{
	m_pFixture->GetBody()->DestroyFixture(m_pFixture);
}
void FixtureComponent::startContact(FixtureComponent* pOther)
{
	m_startCB(pOther);
}
void FixtureComponent::endContact(FixtureComponent* pOther)
{
	m_endCB(pOther);
}
const Vec2 FixtureComponent::getOffset() const
{
	return m_offset;
}
/// <summary>
/// Finds the center in world coordinates of the fixture
/// </summary>
Vec2 FixtureComponent::getCenter() const
{
	Vec2 center(0,0);

	if(m_spShape->GetType() == b2Shape::e_polygon)
	{
		b2PolygonShape* pPShape = static_cast<b2PolygonShape*>(m_spShape.get());

		int num = pPShape->GetVertexCount();
		for(int i = 0; i<num; ++i)
			center += (Vec2)pPShape->GetVertex(i);

		center.x /= num;
		center.y /= num;
		center = m_pFixture->GetBody()->GetWorldPoint(center);
	}
	else if(m_spShape->GetType() == b2Shape::e_circle)//must be a circle
	{
		center = m_pFixture->GetBody()->GetWorldPoint(static_cast<b2CircleShape*>(m_spShape.get())->GetVertex(0));
	}
	else
	{
		std::cout << FILELINE;
		///eRROR LOG
	}
	return center;
}
float FixtureComponent::getAngle() const//RADIANS CCW
{
	float ret = m_pFixture->GetBody()->GetAngle();
	return leon::normRad(ret);
}
int FixtureComponent::getIOPos() const
{
	return m_ioPos;
}
b2Body* FixtureComponent::getBodyPtr()
{
	return m_pFixture->GetBody();
}
const b2Body* FixtureComponent::getBodyPtr() const
{
	return m_pFixture->GetBody();
}
void FixtureComponent::setIOPos(int ioPos)
{
	m_ioPos = ioPos;
}
/// <summary>
/// Applies force to center of body(Newtons)
/// </summary>
void FixtureComponent::applyForce(const Vec2& rForce)//applies force to center of body(Newtons)
{
	if(!game.getUniverse().isPaused())
		m_pFixture->GetBody()->ApplyForceToCenter(rForce, true);
}
/// <summary>
/// Applies force to center of this fixture(Newtons)
/// </summary>
void FixtureComponent::applyForceFixture(const Vec2& rForce)//applies force at the center of fixture(Newtons)
{
	if(!game.getUniverse().isPaused())
		m_pFixture->GetBody()->ApplyForce(rForce, getCenter(), true);
}
/// <summary>
/// applies torque to body(Newton Meters)
/// </summary>
void FixtureComponent::applyTorque(float radiansCCW)//applies torque to body(Newton Meters)
{
	if(!game.getUniverse().isPaused())
		m_pFixture->GetBody()->ApplyTorque(radiansCCW, true);
}
void FixtureComponent::setCategory(Category cat)
{
	b2Filter filter = m_pFixture->GetFilterData();
	filter.categoryBits = static_cast<uint16_t>(cat);
	m_pFixture->SetFilterData(filter);
}
void FixtureComponent::setMask(Mask mask)
{
	b2Filter filter = m_pFixture->GetFilterData();
	filter.maskBits = static_cast<uint16_t>(mask);
	m_pFixture->SetFilterData(filter);
}
void FixtureComponent::setStore(const String& rTargetName)
{
	m_store = rTargetName;
}
const String& FixtureComponent::getStore() const
{
	return m_store;
}
bool FixtureComponent::isSensor() const
{
	return m_pFixture->IsSensor();
}
