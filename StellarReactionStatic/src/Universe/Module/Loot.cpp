#include "Loot.hpp"
#include "Weapon.hpp"
#include "JSON.hpp"
#include "BodyComponent.hpp"
#include "Chunk.hpp"



void LootData::loadJson(const Json::Value& root)
{
	GETJSON(resources);
	SensorData::loadJson(root);
}
Loot::Loot(const LootData& rData) : Sensor(rData)
{
	m_resources = rData.resources;
	m_used = false;
}
Loot::~Loot()
{

}
void Loot::postPhysUpdate()
{

}
/// <summary>
/// when a fixture enters our sensor. called multiple times in a step if multiple bodies entered (in no particular order)
/// </summary>
void Loot::entered(FixtureComponent* pOther)
{
	Chunk* newGuest = pOther->getParentBody()->moduleParent->thisAsChunk();

	if(!m_used)//if it doesn't already contain it
	{
		m_used = false;
		int target = newGuest->m_io.getPosition();
		sf::Packet loot;
		m_resources.intoPacket(&loot);
		loot << m_parent->getFactoryPosition();
		Message giveLoot(target, "pickupLoot", loot, 0, false);
		Message::SendUniverse(giveLoot);
	}
}
/// <summary>
/// when a fixture exits our sensor. called multiple times in a step if multiple bodies entered (in no particular order)
/// </summary>
void Loot::exited(FixtureComponent* pOther)
{

}
void Loot::input(String rCommand, sf::Packet rData)
{
	Module::input(rCommand, rData);
}
