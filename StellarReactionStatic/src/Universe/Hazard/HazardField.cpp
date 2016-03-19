#include "HazardField.hpp"
#include <stdlib.h>
#include <time.h>
#include "Chunk.hpp"
#include "Universe.hpp"
#include "BlueprintLoader.hpp"

HazardField::HazardField(Universe* universe, b2Vec2 origin)
{
	this->universe = universe;
	this->origin = origin;

	timer.setCountDown(5.0);
	timer.getTimeElapsed();
	spawned = false;
}
void HazardField::update()
{

	//if(timer.isTimeUp())
	//{
	//	b2Vec2 displacement = b2Vec2(static_cast<float>(rand() % 5), static_cast<float>(rand() % 5));
	//	b2Vec2 velocity = b2Vec2(static_cast<float>(rand() % 10 - 5), static_cast<float>(rand() % 10 - 5));
	//	//Get the asteroid's blueprint
	//	ChunkData* p = universe->getBlueprints().getChunkSPtr("Asteroid")->clone();
	//	//Move asteroid to a random location


	//	p->bodyComp.coords = origin + displacement;
	//	Chunk* asteroid = p->generate(universe);
	//	//To move :
	//	asteroid->getBodyPtr()->SetLinearVelocity(velocity);
	//	universe->add(asteroid);
	//	timer.restartCountDown();
	//	if (!spawned){
	//		ChunkData* p = universe->getBlueprints().getChunkSPtr("DefaultBlackHole")->clone();
	//		p->bodyComp.coords = b2Vec2(30, -30);
	//		Chunk* hole = p->generate(universe);
	//		universe->add(hole);
	//		spawned = true;
	//	}
	//}

}