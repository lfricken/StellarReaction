#include "Loot.hpp"
#include "Weapon.hpp"
#include "JSON.hpp"
#include "BodyComponent.hpp"



void LootData::loadJson(const Json::Value& root)
{
	GETJSON(resources);
	SensorData::loadJson(root);
}
Loot::Loot(const LootData& rData) : Sensor(rData)
{
	m_resources = rData.resources;
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
	auto newGuest = static_cast<BodyComponent*>(pOther->getBodyPtr()->GetUserData())->parent;



	if(newGuest)
	{
		bool contains = false;

		//check contains
		for(auto it = m_guests.begin(); it != m_guests.end(); ++it)
		{
			auto existingGuest = static_cast<BodyComponent*>((**it).getBodyPtr()->GetUserData())->parent;
			if(existingGuest == newGuest)
			{
				contains = true;
				break;
			}
		}

		if(!contains || m_guests.size() > 1)//if it doesn't already contain it
		{
			int target = newGuest->m_io.getPosition();
			sf::Packet loot;
			m_resources.into(&loot);
			loot << m_parentChunk->universePosition;
			Message giveLoot(target, "pickupLoot", loot, 0, false);
			Message::SendUniverse(giveLoot);
		}
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
