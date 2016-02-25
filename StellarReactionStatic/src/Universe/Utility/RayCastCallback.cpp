#include "RayCastCallback.hpp"

using namespace std;

RayCastCallback::RayCastCallback()
{
	m_colMask = static_cast<uint16_t>(Mask::None);//by default, collide with nothing
	m_pIgnoreBody = NULL;
	m_latest.pFixture = NULL;
	m_latest.point = b2Vec2(-1,-1);
}
RayCastCallback::~RayCastCallback()
{

}
float32 RayCastCallback::ReportFixture(b2Fixture* pFixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
{
	b2Body* pBody = pFixture->GetBody();
	if(m_pIgnoreBody!=pBody && fraction<=m_latest.fraction && !pFixture->IsSensor() && m_colMask&pFixture->GetFilterData().categoryBits)
	{
		m_latest.pFixture = pFixture;
		m_latest.point = point;
		m_latest.fraction = fraction;
		return fraction;
	}
	else
		return m_latest.fraction;
}
void RayCastCallback::setIgnoreBody(b2Body* pBody)
{
	m_pIgnoreBody = pBody;
}
void RayCastCallback::addMask(Mask mask)
{
	m_colMask |= static_cast<uint16_t>(mask);
}
void RayCastCallback::removeMask(Mask mask)
{
	m_colMask &= ~static_cast<uint16_t>(mask);//and it with the bitwise negation
}
const RayData& RayCastCallback::getLatest() const
{
	return m_latest;
}
void RayCastCallback::reset()
{
	m_latest.point = b2Vec2(0,0);
	m_latest.pFixture = NULL;
	m_latest.fraction = 1.f;
}
