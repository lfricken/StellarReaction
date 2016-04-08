#include "stdafx.h"
#include "gtest/gtest.h"
#include "Stealth.hpp"
#include "Universe.hpp"
#include "Controller.hpp"

extern Game game;

TEST(ShipAITest, MovesTowardTarget)
{
	//std::cout << "\nBegin Ship AI Test.";
	//game.runTicks(5);
	//std::cout << "\nThis1.";
	//game.runTicks(5);
	//std::cout << "\nThis2.";
	//auto& list = game.getUniverse().getControllerFactory().m_spControlList;

	//Controller* aiCon = list.back().get();
	//Chunk* aiShip = aiCon->getSlave();

	//b2Vec2 origPos = aiShip->getBodyComponent().getPosition();
	//float origAng = aiShip->getBodyComponent().getBodyPtr()->GetAngle();

	//game.runTicks(30);

	//b2Vec2 newPos = aiShip->getBodyComponent().getPosition();
	//float newAng = aiShip->getBodyComponent().getBodyPtr()->GetAngle();

	//EXPECT_FALSE(origPos.x == newPos.x);
	//EXPECT_FALSE(origPos.y == newPos.y);
	//EXPECT_FALSE(origAng == newAng);
}
