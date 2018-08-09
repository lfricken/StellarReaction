#include "FixtureComponent.hpp"
#include "Globals.hpp"
#include "Convert.hpp"
#include "Universe.hpp"
#include "JSON.hpp"
#include "Debugging.hpp"
#include "BodyComponent.hpp"

void FixtureComponentData::loadJson(const Json::Value& root)
{
	if(!root["shape"].isNull())
	{
		String temp = root["shape"].asString();
		if(temp == "rectangle")
			shape = leon::Shape::Rectangle;
		else if(temp == "circle")
			shape = leon::Shape::Circle;
		else if(temp == "custom")
			shape = leon::Shape::Custom;
		else
		{
			WARNING;
			shape = leon::Shape::Circle;
		}
	}

	GETJSON(offset);
	GETJSON(size);
	GETJSON(mass);
	GETJSON(friction);
	GETJSON(restitution);
	GETJSON(isSensor);
	GETJSON(colCategory);
	GETJSON(colMask);
	GETJSON(vertices);
}
FixtureComponent::FixtureComponent(const FixtureComponentData& rData)
{
	m_parentBody = nullptr;
	m_offset = rData.offset;
	m_ioPos = -1;
	Vec2 size = Convert::universeToWorld(rData.size);
	/**RECTANGLE**/
	if(rData.shape == leon::Shape::Rectangle)
	{
		m_spShape = sptr<b2Shape>(new b2PolygonShape);
		static_cast<b2PolygonShape*>(m_spShape.get())->SetAsBox(size.x / 2.f, size.y / 2.f, Convert::universeToWorld(m_offset), 0);

		m_fixtureDef.density = rData.mass / (size.x * size.y);
	}
	/**CIRCLE**/
	else if(rData.shape == leon::Shape::Circle)
	{
		m_spShape = sptr<b2Shape>(new b2CircleShape);
		b2CircleShape* temp = static_cast<b2CircleShape*>(m_spShape.get());
		temp->m_p.Set(Convert::universeToWorld(m_offset.x), Convert::universeToWorld(m_offset.y));
		temp->m_radius = size.x / 2.f;

		m_fixtureDef.density = rData.mass / (temp->m_radius * temp->m_radius * Math::Pi);
	}
	else if(rData.shape == leon::Shape::Custom)
	{
		m_spShape = sptr<b2Shape>(new b2PolygonShape);
		const int vertCount = rData.vertices.size();
		b2Vec2* vecs = new b2Vec2[vertCount];
		for(int i = 0; i < vertCount; ++i)
		{
			vecs[i] = Convert::universeToWorld(rData.vertices[i]);
		}
		static_cast<b2PolygonShape*>(m_spShape.get())->Set(vecs, vertCount);

		m_fixtureDef.density = rData.mass;
	}

	m_fixtureDef.isSensor = rData.isSensor;
	m_fixtureDef.shape = &*m_spShape;//give our shape to our fixture definition
	m_fixtureDef.friction = rData.friction;
	m_fixtureDef.restitution = rData.restitution;//setting our fixture data
	m_fixtureDef.filter.maskBits = static_cast<uint16_t>(rData.colMask);
	m_fixtureDef.filter.categoryBits = static_cast<uint16_t>(rData.colCategory);

	if(rData.pBody != nullptr)
	{
		m_pFixture = rData.pBody->CreateFixture(&m_fixtureDef);
		m_pFixture->SetUserData(this);
	}
	else
		WARNING;
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
Vec2 FixtureComponent::getCenter() const
{
	Vec2 center(0, 0);

	if(m_spShape->GetType() == b2Shape::e_polygon)
	{
		b2PolygonShape* pPShape = static_cast<b2PolygonShape*>(m_spShape.get());

		int num = pPShape->m_count;
		for(int i = 0; i < num; ++i)
			center += (Vec2)pPShape->m_vertices[i];

		center.x /= num;
		center.y /= num;
		center = m_pFixture->GetBody()->GetWorldPoint(center);
	}
	else if(m_spShape->GetType() == b2Shape::e_circle)
	{
		center = m_pFixture->GetBody()->GetWorldPoint(static_cast<b2CircleShape*>(m_spShape.get())->m_p);
	}
	else
	{
		WARNING;
	}
	return Convert::worldToUniverse(center);
}
int FixtureComponent::getIOPos() const
{
	return m_ioPos;
}
BodyComponent* FixtureComponent::getParentBody() const
{
	if(m_parentBody == nullptr)
		m_parentBody = static_cast<BodyComponent*>(m_pFixture->GetBody()->GetUserData());

	return m_parentBody;
}
void FixtureComponent::setIOPos(int ioPos)
{
	m_ioPos = ioPos;
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
Team FixtureComponent::getTeam() const
{
	return getParentBody()->getTeam();
}
bool FixtureComponent::isSensor() const
{
	return m_pFixture->IsSensor();
}






