#pragma once

#include "stdafx.hpp"
#include "Money.hpp"

/// <summary>
/// Holds information about each human player
/// </summary>
class BasePlayerTraits
{
public:
	BasePlayerTraits(const std::string& rName);
	virtual ~BasePlayerTraits();

	
	void setTeam(int team);//Which team are we on?
	int getTeam() const;//Which team are we on?

	
	virtual void setController(int index);//Which controller does this player send commands to?
	int getController() const;//Which controller does this player send commands to?

	void setShipChoice(const std::string& rTitle);//which ship does this player want to start with?
	const std::string& getShipChoice() const;//which ship does this player want to start with?
	
	/// <summary>
	/// In Game name, such as mightyWarrior23
	/// </summary>
	void setName(const std::string& rTitle);
	/// <summary>
	/// In Game name, such as mightyWarrior23
	/// </summary>
	const std::string& getName() const;

	/// <summary>
	/// This base player trait should represent the local player who is
	/// also the server host, we decided that we should add a module to our list
	/// so we send a message directly to our ship gui editor
	/// </summary>
	/// <param name="newTitle">The new title.</param>
	/// <param name="rPos">The r position.</param>
	virtual void addModule(const std::string& newTitle, const b2Vec2& rPos);
	virtual bool removeModule(const std::string& oldTitle);//remove module from m_owned, returns if it was removed or not

	const std::vector<std::pair<std::string, b2Vec2> >& getOwnedModuleTitles() const;

	void setMoney(Money amount);
	void changeMoney(Money amount);
	Money getMoney() const;
	void setActiveControlGroup(int control_group);
	int getActiveControlGroup() const;

protected:
	int m_controller;//which controller do we have, 0, 1, 2, ect.(points to a controller in the list)
	std::vector<std::pair<std::string, b2Vec2> > m_owned;//list of modules and where they exist on the ship

private:
	std::string m_shipChoice;
	std::string m_name;
	int m_team;
	Money m_balance;
	int m_activeControlGroup;
};
