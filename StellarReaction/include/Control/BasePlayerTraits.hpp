#ifndef BASEPLAYERTRAITS_HPP
#define BASEPLAYERTRAITS_HPP

#include "stdafx.hpp"
#include "Money.hpp"

class BasePlayerTraits
{
public:
	BasePlayerTraits(const std::string& rName);
	virtual ~BasePlayerTraits();

	void setTeam(int team);
	int getTeam() const;

	virtual void setController(int index);//allow player to override it
	int getController() const;

	void setShipChoice(const std::string& rTitle);
	const std::string& getShipChoice() const;

	void setName(const std::string& rTitle);
	const std::string& getName() const;

	void addModule(const std::string& newTitle);
	bool removeModule(const std::string& oldTitle);//remove module, did it get removed?
	const std::vector<std::string>& getOwnedModuleTitles() const;

	void setMoney(Money amount);
	void changeMoney(Money amount);
	Money getMoney() const;
protected:
	int m_controller;//which controller do we have, 0, 1, 2, ect.(points to a controller in the list)
private:

	std::vector<std::string> m_unusedModules;//that aren't on our ship, uses titles

	std::string m_shipChoice;
	std::string m_name;
	int m_team;
	Money m_balance;
};

#endif // BASEPLAYERTRAITS_HPP
