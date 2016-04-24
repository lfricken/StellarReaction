#pragma once

#include "stdafx.hpp"
#include "Money.hpp"


///Holds information about each player.
///Inherited by Player (local player) and Connection (host has one for each human player).
class BasePlayerTraits
{
public:
	BasePlayerTraits(const std::string& rName);
	virtual ~BasePlayerTraits();

	///Set our team.
	void setTeam(int team);
	///Return which team we are on.
	int getTeam() const;


	///Set which controller does this entity control.
	virtual void setController(int index);
	///Which controller does this entity control?
	int getController() const;

	///Set which ship does this player want to start with.
	void setShipChoice(const std::string& rTitle);
	///Which ship does this player want to start with?
	const std::string& getShipChoice() const;
	

	///Set in game name, seen by other players.
	void setName(const std::string& rTitle);
	///Get in game name, seen by other players.
	const std::string& getName() const;

	/// Give this player another module of this type.
	virtual void addModule(const std::string& newTitle, const b2Vec2& rPos);
	/// Take from this player another module of this type.
	virtual bool removeModule(const std::string& oldTitle);

	/// Get the List of modules this player has, as well as their positions.
	const std::vector<std::pair<std::string, b2Vec2> >& getOwnedModuleTitles() const;
	/// Set this players money value.
	void setMoney(Money amount);
	/// Change this players money by a certain amount.
	void changeMoney(Money amount);
	/// Get how much money this player has.
	Money getMoney() const;

protected:
	///Which controller do we have, 0, 1, 2, ect.(points to a controller in the list)
	int m_controller;
	///Llist of modules and where they exist on the ship.
	std::vector<std::pair<std::string, b2Vec2> > m_owned;

private:
	///Name of ship this player wants (blueprint name).
	std::string m_shipChoice;
	///In game name.
	std::string m_name;
	///Which team this player is on.
	int m_team;
	///Total current money this player owns.
	Money m_balance;
};
