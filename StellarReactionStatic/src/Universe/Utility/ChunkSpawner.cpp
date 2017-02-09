#include "ChunkSpawner.hpp"
#include "Chunk.hpp"
#include "Universe.hpp"
#include "BlueprintLoader.hpp"

#include "JSON.hpp"

ChunkSpawner::ChunkSpawner(Universe* universe, const Json::Value& root)
{
	m_pUniverse = universe;
	IOComponentData io(&universe->getUniverseIO());
	LOADJSON(io);

	m_io.reset(new IOComponent(io, &ChunkSpawner::input, this));

	GETJSON(m_isEnabled);
	GETJSON(m_blueprints);
	GETJSON(m_maxNumber);
	GETJSON(m_spawnPeriod);
	GETJSON(m_spawnPeriodVariance);
	GETJSON(m_radius);
	GETJSON(m_origin);
	GETJSON(m_team);

	resetCountdown();
}
void ChunkSpawner::resetCountdown()
{

}
void ChunkSpawner::update()
{
	//if(m_timer.isTimeUp())
	//{
	//	Vec2 randPos = pickRandPoint();

	//	ChunkDataMessage ship;

	//	ship.aiControlled = m_makesAI;
	//	ship.needsController = m_makesAI;
	//	ship.coordinates = randPos;
	//	ship.blueprintName = m_blueprints[Rand::get(0,m_blueprints.size())];
	//	ship.rotation = 0;
	//	ship.team = (int)m_team;
	//	ShipBuilder::Client::createChunk(ship);


	//	m_timer.setCountDown(Rand::get(-m_spawnPeriodVariance, m_spawnPeriodVariance) + m_spawnPeriod);
	//	m_timer.restartCountDown();
	//}

	//for(int i = 0; i < m_maxNumber; ++i)
	//{
	//	float rx = Rand::get(-m_radius, m_radius);
	//	float ry = Rand::get(-m_radius, m_radius);
	//	float32 rotation = Rand::get(0.f, 360.f);

	//	float32 x = m_origin.x + rx;
	//	float32 y = m_origin.y + ry;

	//	sf::Packet pos;
	//	pos << x << y << rotation;

	//	Message makeHazard(m_io.getPosition(), "createHazard", pos, 0, false);
	//	m_pUniverse->getUniverseIO().recieve(makeHazard);
	//}
}
Vec2 ChunkSpawner::pickRandPoint()
{
	Vec2 point;

	Vec2 botLeft(m_origin.x - m_radius, m_origin.y - m_radius);
	Vec2 topRight(m_origin.x + m_radius, m_origin.y + m_radius);

	do
	{
		point.x = Rand::get(botLeft.x, topRight.x);
		point.y = Rand::get(botLeft.y, topRight.y);

	} while((point - m_origin).len() > m_radius);

	return point;
}
void ChunkSpawner::input(String command, sf::Packet data)
{
	//if(command == "createHazard")
	//{
	//	Vec2 pos;
	//	float32 rotation;
	//	data >> x >> y >> rotation;

	//	sptr<ChunkData> data(m_pUniverse->getBlueprints().getChunkSPtr(m_hazardName)->clone());
	//	data->bodyComp.coords = Vec2(x, y);
	//	data->bodyComp.rotation = rotation;

	//	Chunk* asteroid = data->generate(m_pUniverse);
	//	m_pUniverse->add(asteroid);
	//}
}


