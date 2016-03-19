#include "HazardField.hpp"
#include <stdlib.h>
#include <time.h>
#include "Chunk.hpp"
#include "Universe.hpp"
#include "BlueprintLoader.hpp"

HazardField::HazardField(Universe* universe, const Json::Value& root) : m_io(IOComponentData(game.getUniverse().getUniverseIO()), &HazardField::input, this)
{
	m_pUniverse = universe;

	//if(root

	//m_hazardName = hazardName;
	//m_numHazards = numHazards;
	//m_radius = radius;
	//m_origin = origin;
}
void HazardField::spawn()
{
	for(int i = 0; i < m_numHazards; ++i)
	{

		//float randX = 
		//float randY = 

		//b2Vec2 pos = m_origin + b2Vec2(static_cast<float>(rand() % 5), static_cast<float>(rand() % 5));


	}
}
void HazardField::input(std::string command, sf::Packet data)
{
	if(command == "createHazard")
	{
		float32 x;
		float32 y;
		data >> x >> y;

		sptr<ChunkData> data(m_pUniverse->getBlueprints().getChunkSPtr(m_hazardName)->clone());
		data->bodyComp.coords = b2Vec2(x, y);

		Chunk* asteroid = data->generate(m_pUniverse);
		m_pUniverse->add(asteroid);
	}
}
