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

	GETJSON(m_spawnPeriod);
	GETJSON(m_makesAI);
	GETJSON(m_isEnabled);
	GETJSON(m_blueprints);
	GETJSON(m_spawnAmount);

	GETJSON(m_spawnPeriodVariance);
	GETJSON(m_radius);
	GETJSON(m_origin);
	GETJSON(m_team);

	resetCountdown();
}
void ChunkSpawner::resetCountdown()
{
	float randSec = Rand::get(-m_spawnPeriodVariance, m_spawnPeriodVariance);
	m_timer.setCountDown(m_spawnPeriod + randSec);
	m_timer.restartCountDown();
}
void ChunkSpawner::update()
{
	if(m_spawnAmount > 0 && m_timer.isTimeUp())
	{
		ChunkDataMessage ship;

		ship.aiControlled = m_makesAI;
		ship.needsController = m_makesAI;
		ship.coordinates = pickRandPoint();
		ship.blueprintName = m_blueprints[Rand::get(0,m_blueprints.size()-1)];
		ship.rotation = 0;
		ship.team = (int)m_team;
		ShipBuilder::Client::createChunk(ship, 0);
		--m_spawnAmount;
		resetCountdown();
	}
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


