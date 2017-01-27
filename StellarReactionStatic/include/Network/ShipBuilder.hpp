#pragma once
#include "stdafx.hpp"

class BasePlayerTraits;
class Chunk;
/// \brief Handles ship manipulation messages.
class ShipBuilder
{
public:
	ShipBuilder();
	~ShipBuilder();

	/// Input function to decide what to do with a message.
	class Gui
	{
	public:
		static void buyModule(const String& rCommand, sf::Packet& rData, BasePlayerTraits* pFrom);
		static void addModule(const String& rCommand, sf::Packet& rData, BasePlayerTraits* pFrom);
	};

	static bool handleCommand(const String& rCommand, sf::Packet& rData, BasePlayerTraits* pFrom);
	static void rebuild(const String& rCommand, sf::Packet& rData, BasePlayerTraits* pFrom);

	static void extractModules(sf::Packet& rData, List<Pair<String, sf::Vector2i> >* list);
	static void insertModules(sf::Packet& rData, const List<Pair<String, sf::Vector2i> >& list);

	static void attachModule(Chunk* ship, const String& bpName, const sf::Vector2i offset);
	static void cleanShip(const Chunk* ship);

	List<std::pair<String, sf::Vector2i> > getModules();
};

