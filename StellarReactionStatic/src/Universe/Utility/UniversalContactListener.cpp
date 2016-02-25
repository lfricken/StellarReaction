#include "UniversalContactListener.hpp"
#include "FixtureComponent.hpp"

using namespace std;

void UniversalContactListener::BeginContact(b2Contact* contact)
{
	FixtureComponent* fixA = static_cast<FixtureComponent*>(contact->GetFixtureA()->GetUserData());
	FixtureComponent* fixB = static_cast<FixtureComponent*>(contact->GetFixtureB()->GetUserData());

	fixA->startContact(fixB);
	fixB->startContact(fixA);
}
void UniversalContactListener::EndContact(b2Contact* contact)
{
	FixtureComponent* fixA = static_cast<FixtureComponent*>(contact->GetFixtureA()->GetUserData());
	FixtureComponent* fixB = static_cast<FixtureComponent*>(contact->GetFixtureB()->GetUserData());

	fixA->endContact(fixB);
	fixB->endContact(fixA);
}
void UniversalContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	//(void)oldManifold;//shutup the compiler about unused
	//PhysicsBase* fixA = static_cast<PhysicsBase*>(contact->GetFixtureA()->GetUserData());
	//PhysicsBase* fixB = static_cast<PhysicsBase*>(contact->GetFixtureB()->GetUserData());

	//std::cout << "\PreSolve.";

	//fixA->preSolveContact(fixB);
	//fixB->preSolveContact(fixA);
}
void UniversalContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	//(void)impulse;//shutup the compiler about unused
	//PhysicsBase* fixA = static_cast<PhysicsBase*>(contact->GetFixtureA()->GetUserData());
	//PhysicsBase* fixB = static_cast<PhysicsBase*>(contact->GetFixtureB()->GetUserData());

	//std::cout << "\PostSolve.";

	//fixA->postSolveContact(fixB);
	//fixB->postSolveContact(fixA);
}
