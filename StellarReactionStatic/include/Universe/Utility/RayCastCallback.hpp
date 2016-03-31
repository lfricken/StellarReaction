#pragma once

#include "stdafx.hpp"
#include "CollisionCategory.hpp"

struct RayData
{
	b2Fixture* pFixture;//fixture we collided with
	b2Vec2 point;//point of collision
	//fraction of initial endpoint distance, 1 to 2 with a collision of 3 would be a fraction of 2
	//1 to 2 with endpoint 1.5 is a fraction of 0.5 (except all in 2d not 1d)
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

	const Map<float, RayData>& getLatest() const;
	void reset();

protected:
private:
	uint16_t m_colMask;//only collide with this stuff
	b2Body* m_pIgnoreBody;//ignore this body
	std::function<void(b2Fixture* pFixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)> m_cbFunction;//the function we call when we get a receive call
	Map<float, RayData> m_collisions;
};
