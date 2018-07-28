#pragma once

#include "stdafx.hpp"
#include "CollisionCategory.hpp"
#include "NonCopyable.hpp"

class BodyComponent;
enum class Team;

namespace leon
{
	enum class Shape
	{
		Rectangle,
		Circle,
		Custom,
	};
}

/// Initializes FixtureComponent
struct FixtureComponentData
{
	FixtureComponentData() :
		offset(0,0),
		pBody(nullptr),
		shape(leon::Shape::Circle),
		size(1,1),
		mass(1.f),
		friction(0.3f),
		restitution(0.3f),
		isSensor(false),
		colCategory(Category::ShipModule),
		colMask(Mask::ShipModule)
	{

	}
	Vec2 offset;//offset from center of body
	b2Body* pBody;//who owns us
	leon::Shape shape;

	Vec2 size;//width and height in world coordinates
	float mass;
	float friction;
	float restitution;//how bouncy are we?
	bool isSensor;//are we a sensor

	Category colCategory;//what are we?
	Mask colMask;//what do we collide with
	List<Vec2> vertices;

	///Fill this object with data from a json file.
	void loadJson(const Json::Value& root);
};



/// Wrapper for Box2D Fixture.
class FixtureComponent : NonCopyable
{
public:
	FixtureComponent(const FixtureComponentData& rData);
	virtual ~FixtureComponent();
	/// Returns if this object is a sensor.
	bool isSensor() const;
	/// <summary>
	/// Returns the current center position.
	/// </summary>
	Vec2 getCenter() const;
	/// Returns the angle in RADCCW of the fixture.
	float getAngle() const;	
	/// Gets the io position of our owner.
	int getIOPos() const;
	/// <summary>
	/// 
	/// </summary>
	BodyComponent* getParentBody() const;
	/// Returns a const pointer to the box2D body pointer.
	//const b2Body* getBodyPtr() const;
	/// <summary>
	/// Center of this fixture with respect to the body.
	/// </summary>
	const Vec2 getOffset() const;
	/// Applies the given force to the fixture.
	void applyForceFixture(const Vec2& rForce);
	/// Sets the category of the FixtureComponent.
	void setCategory(Category cat);
	/// Sets the mask of the FixtureComponent.
	void setMask(Mask mask);
	/// <summary>
	/// 
	/// </summary>
	Team getTeam() const;

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

	String m_store;
	Vec2 m_offset;
};
