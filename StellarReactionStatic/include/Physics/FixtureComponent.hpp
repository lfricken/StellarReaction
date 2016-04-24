#pragma once

#include "stdafx.hpp"
#include "CollisionCategory.hpp"
#include "NonCopyable.hpp"

namespace leon
{
	enum class Shape
	{
		Rectangle,
		Circle,
	};
}

struct FixtureComponentData
{
	FixtureComponentData() :
		offset(0,0),
		pBody(NULL),
		shape(leon::Shape::Rectangle),
		size(1,1),
		density(1.f),
		friction(0.3f),
		restitution(0.3f),
		isSensor(false),
		colCategory(Category::ShipModule),
		colMask(Mask::ShipModule)
	{

	}
	b2Vec2 offset;//offset from center of body
	b2Body* pBody;//who owns us
	leon::Shape shape;

	b2Vec2 size;//width and height in world coordinates
	float density;
	float friction;
	float restitution;//how bouncy are we?
	bool isSensor;//are we a sensor

	Category colCategory;//what are we?
	Mask colMask;//what do we collide with
	///Fill this object with data from a json file.
	void loadJson(const Json::Value& root);
};

/*
* FixtureComponent Class:
* Creates FixtureComponent object.
* FixtureComponent objects define bodies in the game world.
*/

class FixtureComponent : NonCopyable
{
public:
	FixtureComponent(const FixtureComponentData& rData);
	virtual ~FixtureComponent();
	/// Returns if this object is a sensor.
	bool isSensor() const;
	/// Returns the current center position.
	b2Vec2 getCenter() const;
	/// Returns the current angle.
	float getAngle() const;//RADIANS CCW	
	/// Gets the io position of our owner.
	int getIOPos() const;
	/// Returns a pointer to the box2D body pointer.
	b2Body* getBodyPtr();
	/// Returns a const pointer to the box2D body pointer.
	const b2Body* getBodyPtr() const;
	/// Returns a const offset vector.
	const b2Vec2 getOffset() const;
	/// Applies the given force to the center of the body.
	void applyForce(const b2Vec2& rForce);
	/// Applies the given force to the fixture.
	void applyForceFixture(const b2Vec2& rForce);
	/// Applies the given rotation to the body.
	void applyTorque(float radiansCCW);
	/// Sets the category of the FixtureComponent.
	void setCategory(Category cat);
	/// Sets the mask of the FixtureComponent.
	void setMask(Mask mask);
	/// Sets the store.
	void setStore(const std::string& rTargetName);
	/// Returns the store.
	const std::string& getStore() const;

protected:
private:
	friend class UniversalContactListener;
	friend class Module;
	template <typename T>
	void bindStartCB(void (T::*func)(FixtureComponent*), T* const classPtr)//because C++
	{
		m_startCB = std::bind(func, classPtr, std::placeholders::_1);
	}
	template <typename T>
	void bindEndCB(void (T::*func)(FixtureComponent*), T* const classPtr)//because C++
	{
		m_endCB = std::bind(func, classPtr, std::placeholders::_1);
	}
	void setIOPos(int ioPos);


	void startContact(FixtureComponent* pOther);
	void endContact(FixtureComponent* pOther);

	std::function<void(FixtureComponent*)> m_startCB;//other component, their IO position
	std::function<void(FixtureComponent*)> m_endCB;//other component, their IO position

	sptr<b2Shape> m_spShape;
	b2FixtureDef m_fixtureDef;
	b2Fixture* m_pFixture;
	int m_ioPos;

	std::string m_store;
	b2Vec2 m_offset;
};
