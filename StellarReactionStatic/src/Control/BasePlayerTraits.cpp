#include "BasePlayerTraits.hpp"
#include "Message.hpp"
#include "Globals.hpp"
#include "IOManager.hpp"

using namespace std;

BasePlayerTraits::BasePlayerTraits(const String& rName)
{
	m_team = Team::One;
	m_shipChoice = "Anubis";
	m_name = rName;
	m_balance = 0;
	m_controller = -1;
}
BasePlayerTraits::~BasePlayerTraits()
{

}
void BasePlayerTraits::setShipChoice(const String& rTitle)
{
	m_shipChoice = rTitle;
}
const String&  BasePlayerTraits::getShipChoice() const
{
	return m_shipChoice;
}
void BasePlayerTraits::setName(const String& rTitle)
{
	m_name = rTitle;
}
const String& BasePlayerTraits::getName() const
{
	return m_name;
}
void BasePlayerTraits::setTeam(Team team)
{
	m_team = team;
}
Team BasePlayerTraits::getTeam() const
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
void BasePlayerTraits::addModule(const String& newTitle, const Vec2& rPos)
{
	//m_owned.push_back(pair<String, Vec2>(newTitle, rPos));
	sf::Packet pack;
	pack << newTitle;
	pack << (float)rPos.x;
	pack << (float)rPos.y;
	Message modAdded("ship_editor", "addItem", pack, 0.f, false);
	game.getCoreIO().recieve(modAdded);
}

