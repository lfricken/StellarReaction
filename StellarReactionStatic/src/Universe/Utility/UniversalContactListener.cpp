#include "UniversalContactListener.hpp"
#include "FixtureComponent.hpp"

void UniversalContactListener::BeginContact(b2Contact* contact)
{
	//b2WorldManifold man;
	//contact->GetWorldManifold(&man);

	//Vec2 p1(man.points[0]);
	//Vec2 p2(man.points[1]);

	//Print << man.points[0].x << "\n";
	//Print << man.points[1].x << "\n";
	//Print << Vec2(man.normal) << "\n";
	//Print << man.separations[0] << "\n";
	//Print << man.separations[1] << "\n";

	FixtureComponent* fixA = static_cast<FixtureComponent*>(contact->GetFixtureA()->GetUserData());
	FixtureComponent* fixB = static_cast<FixtureComponent*>(contact->GetFixtureB()->GetUserData());

	fixA->startContact(fixB);
	fixB->startContact(fixA);
}
void UniversalContactListener::EndContact(b2Contact* contact)
{
	FixtureComponent* fixA = static_cast<FixtureComponent*>(contact->GetFixtureA()->GetUserData());
	FixtureComponent* fixB = static_cast<FixtureComponent*>(contact->GetFixtureB()->GetUserData());

	//b2WorldManifold man;
	//contact->GetWorldManifold(&man);

	//Vec2 p1(man.points[0]);
	//Vec2 p2(man.points[1]);

	//Print << man.points[0].x << "\n";


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
