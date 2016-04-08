#include "BasePlayerTraits.hpp"
#include "Message.hpp"
#include "Globals.hpp"
#include "IOManager.hpp"

using namespace std;

BasePlayerTraits::BasePlayerTraits(const std::string& rName)
{
	m_team = 1;
	m_shipChoice = "Anubis";
	m_name = rName;
	m_balance = 0;
	m_controller = -1;
}
BasePlayerTraits::~BasePlayerTraits()
{

}
void BasePlayerTraits::setShipChoice(const std::string& rTitle)
{
	m_shipChoice = rTitle;
}
const std::string&  BasePlayerTraits::getShipChoice() const
{
	return m_shipChoice;
}
void BasePlayerTraits::setName(const std::string& rTitle)
{
	m_name = rTitle;
}
const std::string& BasePlayerTraits::getName() const
{
	return m_name;
}
void BasePlayerTraits::setTeam(int team)
{
	m_team = team;
}
int BasePlayerTraits::getTeam() const
{
	return m_team;
}
void BasePlayerTraits::setMoney(Money amount)
{
	m_balance = amount;
}
void BasePlayerTraits::changeMoney(Money amount)
{
	m_balance += amount;
}
Money BasePlayerTraits::getMoney() const
{
	return m_balance;
}
void BasePlayerTraits::setController(int index)
{
	m_controller = index;
}
int BasePlayerTraits::getController() const
{
	return m_controller;
}
void BasePlayerTraits::addModule(const std::string& newTitle, const b2Vec2& rPos)
{
	m_owned.push_back(pair<string, b2Vec2>(newTitle, rPos));
	sf::Packet pack;
	pack << newTitle;
	pack << (float)rPos.x;
	pack << (float)rPos.y;
	Message modAdded("ship_editor", "addItem", pack, 0.f, false);
	game.getCoreIO().recieve(modAdded);
}
bool BasePlayerTraits::removeModule(const std::string& oldTitle)
{
	for(auto it = m_owned.begin(); it != m_owned.end(); ++it)
		if(it->first == oldTitle)
		{
			m_owned.erase(it);
			return true;
		}

	return false;
}
const std::vector<std::pair<std::string, b2Vec2> >& BasePlayerTraits::getOwnedModuleTitles() const
{
	return m_owned;
}
