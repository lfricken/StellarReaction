#pragma once

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

	virtual void addModule(const std::string& newTitle, const b2Vec2& rPos);
	virtual bool removeModule(const std::string& oldTitle);//remove module, did it get removed?

	const std::vector<std::pair<std::string, b2Vec2> >& getOwnedModuleTitles() const;

	void setMoney(Money amount);
	void changeMoney(Money amount);
	Money getMoney() const;

protected:
	int m_controller;//which controller do we have, 0, 1, 2, ect.(points to a controller in the list)
	std::vector<std::pair<std::string, b2Vec2> > m_owned;//they exist

private:
	std::string m_shipChoice;
	std::string m_name;
	int m_team;
	Money m_balance;
};
