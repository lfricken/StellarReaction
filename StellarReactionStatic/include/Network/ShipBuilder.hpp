#pragma once
#include "stdafx.hpp"

class BasePlayerTraits;

/// \brief Manages network Connections, and sending and recieving data.
class ShipBuilder
{
public:
	ShipBuilder();
	~ShipBuilder();

	static bool handleCommand(const String& rCommand, sf::Packet& rData, BasePlayerTraits* pFrom);
	static void rebuild(const String& rCommand, sf::Packet& rData, BasePlayerTraits* pFrom);
	static void buyModule(const String& rCommand, sf::Packet& rData, BasePlayerTraits* pFrom);
	static void addModule(const String& rCommand, sf::Packet& rData, BasePlayerTraits* pFrom);

	List<std::pair<String, sf::Vector2f> > getModules();
};

