#include "BasePlayerTraits.hpp"

BasePlayerTraits::BasePlayerTraits(const std::string& rName)
{
	m_team = 1;
	m_shipChoice = "CombatShip";
	m_name = rName;
	m_balance = 2;
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
void BasePlayerTraits::addModule(const std::string& newTitle)
{
	m_unusedModules.push_back(newTitle);
}
bool BasePlayerTraits::removeModule(const std::string& oldTitle)
{
	for(auto it = m_unusedModules.begin(); it != m_unusedModules.end(); ++it)
	{
		if(*it == oldTitle)
		{
			m_unusedModules.erase(it);
			return true;
		}
	}
	return false;
}
const std::vector<std::string>& BasePlayerTraits::getOwnedModuleTitles() const
{
	return m_unusedModules;
}