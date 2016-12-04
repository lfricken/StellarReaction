#pragma once

#include "stdafx.hpp"
#include "Money.hpp"
#include "Team.hpp"

/// \brief Holds information about each player.
/// 
/// Inherited by Player (local player) and Connection (host has one for each human player).
class BasePlayerTraits
{
public:
	BasePlayerTraits(const String& rName);
	virtual ~BasePlayerTraits();

	///Set our team.
	void setTeam(Team team);
	///Return which team we are on.
	Team getTeam() const;


	///Set which controller does this entity control.
	virtual void setController(int index);
	///Which controller does this entity control?
	int getController() const;

	///Set which ship does this player want to start with.
	void setShipChoice(const String& rTitle);
	///Which ship does this player want to start with?
	const String& getShipChoice() const;
	

	///Set in game name, seen by other players.
	void setName(const String& rTitle);
	///Get in game name, seen by other players.
	const String& getName() const;

	/// Give this player another module of this type.
	virtual void addModule(const String& newTitle, const Vec2& rPos);
	/// Take from this player another module of this type.
	virtual bool removeModule(const String& oldTitle);

	/// Get the List of modules this player has, as well as their positions.
	const List<std::pair<String, Vec2> >& getOwnedModuleTitles() const;
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
	List<std::pair<String, Vec2> > m_owned;

private:
	///Name of ship this player wants (blueprint name).
	String m_shipChoice;
	///In game name.
	String m_name;
	///Which team this player is on.
	Team m_team;
	///Total current money this player owns.
	Money m_balance;
};
