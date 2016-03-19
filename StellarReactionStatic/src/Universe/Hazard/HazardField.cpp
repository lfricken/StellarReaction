#include "HazardField.hpp"
#include "Chunk.hpp"
#include "Universe.hpp"
#include "BlueprintLoader.hpp"
#include "Random.hpp"
#include "JSON.hpp"

HazardField::HazardField(Universe* universe, const Json::Value& root) : m_io(IOComponentData(game.getUniverse().getUniverseIO()), &HazardField::input, this)
{
	m_pUniverse = universe;

	m_hazardName = JSON::get(root, "HazardName", "Asteroid");
	m_numHazards = JSON::get(root, "NumHazards", 1);
	m_radius = JSON::get(root, "Radius", 5.f);
	m_origin.x = JSON::get(root, "OriginX", 0.f);
	m_origin.y = JSON::get(root, "OriginY", 0.f);
}
void HazardField::spawn()
{
	for(int i = 0; i < m_numHazards; ++i)
	{
		float32 x = m_origin.x + Random::getRandom(-m_radius, m_radius);
		float32 y = m_origin.y + Random::getRandom(-m_radius, m_radius);

		sf::Packet pos;
		pos << x << y;

		Message makeHazard(m_io.getPosition(), "createHazard", pos, 0, false);
		game.getUniverse().getUniverseIO().recieve(makeHazard);
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


