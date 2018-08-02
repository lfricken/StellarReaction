#include "IOComponent.hpp"
#include "Globals.hpp"
#include "Universe.hpp"
#include "JSON.hpp"
#include "Debugging.hpp"

IOComponent::~IOComponent()
{
	m_pManager->freeThis(m_ioManPosition);
}
void IOComponent::event(EventType type, int value, const sf::Packet& rData)
{
	m_eventer.event(type, value, rData);
}
void IOComponent::recieve(const String& rCommand, const sf::Packet& rData)
{
	m_cbFunction(rCommand, rData);
}
const String& IOComponent::getName() const
{
	return m_name;
}
int IOComponent::getPosition() const
{
	return m_ioManPosition;
}
Eventer& IOComponent::getEventer()
{
	return m_eventer;
}
void IOComponentData::loadJson(const Json::Value& root)
{
	GETJSON(name);
	if(!root["courierList"].isNull())
	{
		const Json::Value courierListJson = root["courierList"];
		for(auto it = courierListJson.begin(); it != courierListJson.end(); ++it)
		{
			Courier c;

			String target = (*it)["message"]["target"].asString();
			String command = (*it)["message"]["command"].asString();

			sf::Packet packData;
			const Json::Value dataList = (*it)["message"]["data"];
			if(dataList.size() % 2 == 0)//if it's divisible by two
				for(auto it = dataList.begin(); it != dataList.end(); ++it)
				{
					String type = it->asString();

					++it;
					if(type == "bool")
						packData << it->asBool();
					else if(type == "int")
						packData << it->asInt();
					else if(type == "float")
						packData << it->asFloat();
					else if(type == "string")
						packData << it->asString();
					else
						WARNING;
				}
			else
			{
				WARNING;
			}

			float delay = (*it)["message"]["delay"].asFloat();
			bool sendValue = (*it)["message"]["sendValue"].asBool();
			bool sendOverNW = (*it)["message"]["sendOverNW"].asBool();
			c.message.m_replaceTargetPos = (*it)["message"]["replaceTargetPosition"].asBool();
			c.message.reset(target, command, packData, delay, sendValue);
			c.message.sendOverNW(sendOverNW);


			EventType event = ChooseEvent((*it)["condition"]["event"].asString());
			int value = (*it)["condition"]["value"].asInt();
			char comp = (*it)["condition"]["comparator"].asString()[0];
			bool repeatable = (*it)["condition"]["repeatable"].asBool();
			c.condition.reset(event, value, comp, repeatable);

			courierList.push_back(c);
		}
	}
}