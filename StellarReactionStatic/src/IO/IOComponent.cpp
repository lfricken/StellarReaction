#include "IOComponent.hpp"
#include "Globals.hpp"

using namespace std;

IOComponent::~IOComponent()
{
	m_rManager.free(m_ioManPosition);
}
void IOComponent::event(EventType type, int value, const sf::Packet& rData)
{
	m_eventer.event(type, value, rData);
}
void IOComponent::recieve(const std::string& rCommand, const sf::Packet& rData)
{
	m_cbFunction(rCommand, rData);
}
const std::string& IOComponent::getName() const
{
	return m_name;
}
int IOComponent::getPosition() const
{
	return m_ioManPosition;
}
void IOComponentData::loadJson(const Json::Value& root)
{
	if(!root["name"].isNull())
		name = root["name"].asString();
	if(!root["courierList"].isNull())
	{
		const Json::Value courierListJson = root["courierList"];
		for(auto it = courierListJson.begin(); it != courierListJson.end(); ++it)
		{
			Courier c;

			string target = (*it)["message"]["target"].asString();
			string command = (*it)["message"]["command"].asString();

			sf::Packet packData;
			const Json::Value dataList = (*it)["message"]["data"];
			if(dataList.size() % 2 == 0)//if it's divisible by two
				for(auto it = dataList.begin(); it != dataList.end(); ++it)
				{
					string type = it->asString();

					++it;
					if(type == "bool")
						packData << it->asBool();
					if(type == "int")
						packData << it->asInt();
					if(type == "float")
						packData << it->asFloat();
					if(type == "string")
						packData << it->asString();
				}
			else
			{
				cout << "\n" << FILELINE;
				///ERROR LOG
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