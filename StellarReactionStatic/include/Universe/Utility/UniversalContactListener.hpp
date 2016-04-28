#ifndef UNIVERSALCONTACTLISTENER_H
#define UNIVERSALCONTACTLISTENER_H

#include "stdafx.hpp"


/// Used to call callback functions on the FixtureComponent.
class UniversalContactListener : public b2ContactListener
{
public:
	///Begins contact between two objects.
	void BeginContact(b2Contact* contact);
	///Ends contact between two objects.
	void EndContact(b2Contact* contact);
	///Actions to take before solving contact.
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	///Actions to take after solving contact.
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

protected:
private:
};

#endif // UNIVERSALCONTACTLISTENER_H
