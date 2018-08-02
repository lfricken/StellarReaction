#include "BasePlayerTraits.hpp"
#include "Message.hpp"
#include "Globals.hpp"
#include "IOManager.hpp"

BasePlayerTraits::BasePlayerTraits(const String& rName)
{
	m_team = Team::One;
	m_shipChoice = "Anubis";
	m_name = rName;
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
void BasePlayerTraits::setController(int index)
{
	m_controller = index;
}
//void BasePlayerTraits::addModule(const String& newTitle, const sf::Vector2i& rPos)
//{
//	//m_owned.push_back(pair<String, Vec2>(newTitle, rPos));
//	sf::Packet pack;
//	pack << newTitle;
//	pack << rPos.x;
//	pack << rPos.y;
//	Message modAdded("ship_editor", "addModuldeToGui", pack, 0.f, false);
//	getGame()->getCoreIO().recieve(modAdded);
//}

