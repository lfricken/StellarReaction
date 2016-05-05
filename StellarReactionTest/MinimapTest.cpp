#include "stdafx.h"
#include "gtest/gtest.h"
#include "Chunk.hpp"
#include "Game.hpp"
#include "Minimap.hpp"
#include "Universe.hpp"
#include "NetworkBoss.hpp"
#include "BlueprintLoader.hpp"
#include "Player.hpp"

extern Game game;

TEST(MiniMapTest, NumberOfMarkers)
{
	game.restartTest("Alpha Centauri");

	game.runTicks(3);

	// Make sure the number of items on minimap is equal to the number of things,
	// that it should be detecting.
	int totalItems = game.getUniverse().getgoList().size();
	int radarItems = game.getLocalPlayer().radarsize();

	EXPECT_LE(totalItems, radarItems);
}

TEST(MiniMapTest, DotColorEnemy)
{
	game.restartTest("Alpha Centauri");

	sptr<Minimap> minimap;
	MinimapData mapData;
	mapData.layer = GraphicsLayer::OverlayMiddle;
	minimap.reset(new Minimap(mapData));
	b2Vec2 center;

	minimap->setDot(center, 0, 0);

	game.runTicks(3);

	sptr<QuadComponent> dot = minimap->getDot(0);

	sf::Color dotColor = dot->getColor();

	//Make sure dot set to be "enemy" appears red.
	EXPECT_EQ(dotColor, sf::Color::Red);
}

TEST(MiniMapTest, DotColorTeam)
{
	game.restartTest("Alpha Centauri");

	sptr<Minimap> minimap;
	MinimapData mapData;
	mapData.layer = GraphicsLayer::OverlayMiddle;
	minimap.reset(new Minimap(mapData));
	b2Vec2 center;

	minimap->setDot(center, 0, 1);

	game.runTicks(3);

	sptr<QuadComponent> dot = minimap->getDot(0);

	sf::Color dotColor = dot->getColor();
	//Make sure dot set to be "friendly" appears green.
	EXPECT_EQ(dotColor, sf::Color::Green);
}

TEST(MiniMapTest, DotColorOther)
{
	game.restartTest("Alpha Centauri");

	sptr<Minimap> minimap;
	MinimapData mapData;
	mapData.layer = GraphicsLayer::OverlayMiddle;
	minimap.reset(new Minimap(mapData));
	b2Vec2 center;

	minimap->setDot(center, 0, 2);

	game.runTicks(3);

	sptr<QuadComponent> dot = minimap->getDot(0);

	sf::Color dotColor = dot->getColor();
	//Make sure dot set to be "neutral" appears blue.
	EXPECT_EQ(dotColor, sf::Color::Blue);
}