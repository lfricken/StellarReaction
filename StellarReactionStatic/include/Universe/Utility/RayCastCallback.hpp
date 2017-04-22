#pragma once

#include "stdafx.hpp"
#include "CollisionCategory.hpp"

/// Data returned by a RayCastCallback.
struct RayData
{
	b2Fixture* pFixture;//fixture we collided with
	Vec2 point;//point of collision
	//fraction of initial endpoint distance, 1 to 2 with a collision of 3 would be a fraction of 2
	//1 to 2 with endpoint 1.5 is a fraction of 0.5 (except all in 2d not 1d)
};

/// Used to cast a ray through the world and find things that collide with it. Used by LaserWeapon.
class RayCastCallback : public b2RayCastCallback
{
public:
	RayCastCallback();
	virtual ~RayCastCallback();
	///Attempts to collide List with given fixture.
	virtual float32 ReportFixture(b2Fixture* pFixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);
	///Set body to be ignored by raycasting.
	void setIgnoreBody(b2Body* pBody);
	///Adds a mask when raycasting.
	void addMask(Mask ignore);
	///Removes a mask when raycasting.
	void removeMask(Mask ignore);
	///Returns a map of collisions and the RayData for each collision.
	const Map<float, RayData>& getLatest() const;
	///Resets map of collisions.
	void reset();

protected:
private:
	uint16_t m_colMask;//only collide with this stuff
	b2Body* m_pIgnoreBody;//ignore this body
	std::function<void(b2Fixture* pFixture, const Vec2& point, const Vec2& normal, float32 fraction)> m_cbFunction;//the function we call when we get a receive call
	Map<float, RayData> m_collisions;
};
