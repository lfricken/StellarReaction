#include "RayCastCallback.hpp"
#include "Convert.hpp"


RayCastCallback::RayCastCallback()
{
	m_colMask = static_cast<uint16_t>(Mask::None);//by default, collide with nothing
	m_pIgnoreBody = nullptr;
}
RayCastCallback::~RayCastCallback()
{

}
float32 RayCastCallback::ReportFixture(b2Fixture* pFixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
{
	if((m_pIgnoreBody != pFixture->GetBody()) && (m_colMask & pFixture->GetFilterData().categoryBits))
	{
		RayData data;
		data.pFixture = pFixture;
		data.point = Convert::worldToUniverse((Vec2)point);
		m_collisions[fraction] = data;
	}

	return 1.f;
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
const Map<float, RayData>& RayCastCallback::getLatest() const
{
	return m_collisions;
}
void RayCastCallback::reset()
{
	m_collisions.clear();
}
