

#include "Hazard\HazardField.hpp"
#include <stdlib.h>
#include <time.h>
#include "Chunk.hpp"
#include "Universe.hpp"
#include "BlueprintLoader.hpp"

HazardField::HazardField(Universe* universe, b2Vec2 origin){
	this->universe = universe;
	this->origin = origin;
	srand(time(NULL));
	timer.setCountDown(5.0);
	timer.getTimeElapsed();
}

void HazardField::update()
{
	

	if (timer.isTimeUp()){
		std::cout << "Inserting asteroid!" << std::endl;
		b2Vec2 displacement = b2Vec2(rand() % 5, rand() % 5);
		b2Vec2 velocity = b2Vec2((rand() % 10 - 5), (rand() % 10 - 5));

		ChunkData* p = universe->getBlueprints().getChunkSPtr("Asteroids")->clone();

		p->bodyComp.coords = origin + displacement;

		Chunk* asteroid = p->generate(universe);

		//To move :
		asteroid->getBodyPtr()->SetLinearVelocity(velocity);

		universe->add(asteroid);

		timer.restartCountDown();
	}




}