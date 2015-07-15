#ifndef RAYCASTCALLBACK_HPP
#define RAYCASTCALLBACK_HPP

#include "stdafx.hpp"
#include "CollisionCategory.hpp"

struct RayData
{
	b2Fixture* pFixture;
	b2Vec2 point;
	float fraction;
};

class RayCastCallback : public b2RayCastCallback
{
public:
	RayCastCallback();
	virtual ~RayCastCallback();

	virtual float32 ReportFixture(b2Fixture* pFixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);
	void setIgnoreBody(b2Body* pBody);
	void addMask(Mask ignore);
	void removeMask(Mask ignore);

	const RayData& getLatest() const;
	void reset();

protected:
private:
	uint16_t m_colMask;//only collide with this stuff
	b2Body* m_pIgnoreBody;//ignore this body
	std::function<void(b2Fixture* pFixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)> m_cbFunction;//the function we call when we get a receive call
	RayData m_latest;
};

#endif // RAYCASTCALLBACK_HPP
