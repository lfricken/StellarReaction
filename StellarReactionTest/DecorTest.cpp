#include "stdafx.h"
#include "gtest/gtest.h"
#include "Stealth.hpp"
#include "Universe.hpp"
#include "BlueprintLoader.hpp"
//#include "StaticDecor.hpp"
//#include "DynamicDecor.hpp"
//
//extern Game game;
//
//TEST(DecorTest, NoDecor)
//{
//	game.restartTest();
//	int origSize = game.getUniverse().getDecorList().size();
//	game.getUniverse().getBlueprints().loadBlueprints("StellarReactionTest/testblueprints/");
//	std::vector<sptr<Decoration> > dList = game.getUniverse().getDecorList();
//
//	EXPECT_EQ(origSize, dList.size());
//}
//TEST(DecorTest, AddOneStatic)
//{
//	game.restartTest();
//
//	int origSize = game.getUniverse().getDecorList().size();
//	game.getUniverse().getBlueprints().loadBlueprints("StellarReactionTest/testblueprints/");
//	auto thing = game.getUniverse().getBlueprints().getStaticDecorSPtr("test_static")->clone();
//	thing->ioComp.pMyManager = &game.getUniverse().getUniverseIO();
//	game.getUniverse().add(thing->generate());
//	std::vector<sptr<Decoration> > dList = game.getUniverse().getDecorList();
//
//	EXPECT_EQ(origSize+1, dList.size());
//}
//TEST(DecorTest, AddManyStatic)
//{
//	game.restartTest();
//
//	int origSize = game.getUniverse().getDecorList().size();
//	game.getUniverse().getBlueprints().loadBlueprints("StellarReactionTest/testblueprints/");
//	auto thing = game.getUniverse().getBlueprints().getStaticDecorSPtr("test_static")->clone();
//	thing->ioComp.pMyManager = &game.getUniverse().getUniverseIO();
//	for (int i = 0; i < 10; i++)
//	{
//		game.getUniverse().add(thing->generate());
//	}
//	std::vector<sptr<Decoration> > dList = game.getUniverse().getDecorList();
//	
//	EXPECT_EQ(origSize+10, dList.size());
//}
//
//TEST(DecorTest, AddOneDynamic)
//{
//	game.restartTest();
//
//	int origSize = game.getUniverse().getDecorList().size();
//	game.getUniverse().getBlueprints().loadBlueprints("StellarReactionTest/testblueprints/");
//	auto thing = game.getUniverse().getBlueprints().getDynamicDecorSPtr("test_dynamic")->clone();
//	thing->ioComp.pMyManager = &game.getUniverse().getUniverseIO();
//	game.getUniverse().add(thing->generate());
//	std::vector<sptr<Decoration> > dList = game.getUniverse().getDecorList();
//
//	EXPECT_EQ(origSize + 1, dList.size());
//}
//
//TEST(DecorTest, AddManyDynamic)
//{
//	game.restartTest();
//
//	int origSize = game.getUniverse().getDecorList().size();
//	game.getUniverse().getBlueprints().loadBlueprints("StellarReactionTest/testblueprints/");
//	auto thing = game.getUniverse().getBlueprints().getDynamicDecorSPtr("test_dynamic")->clone();
//	thing->ioComp.pMyManager = &game.getUniverse().getUniverseIO();
//	for (int i = 0; i < 10; i++)
//	{
//		game.getUniverse().add(thing->generate());
//	}
//	std::vector<sptr<Decoration> > dList = game.getUniverse().getDecorList();
//
//	EXPECT_EQ(origSize + 10, dList.size());
//}
