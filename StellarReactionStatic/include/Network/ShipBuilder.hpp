#pragma once
#include "stdafx.hpp"

class BasePlayerTraits;

/// \brief Handles ship manipulation messages.
class ShipBuilder
{
public:
	ShipBuilder();
	~ShipBuilder();

	/// Input function to decide what to do with a message.
	static bool handleCommand(const String& rCommand, sf::Packet& rData, BasePlayerTraits* pFrom);
	static void rebuild(const String& rCommand, sf::Packet& rData, BasePlayerTraits* pFrom);
	static void buyModule(const String& rCommand, sf::Packet& rData, BasePlayerTraits* pFrom);
	static void addModule(const String& rCommand, sf::Packet& rData, BasePlayerTraits* pFrom);

	static void extractModules(sf::Packet& rData, List<std::pair<String, sf::Vector2f> >* list);
	static void insertModules(sf::Packet& rData, const List<std::pair<String, sf::Vector2f> >& list);

	List<std::pair<String, sf::Vector2f> > getModules();
};

