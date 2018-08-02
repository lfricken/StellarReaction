#pragma once

#include "stdafx.hpp"
#include "Money.hpp"
#include "Team.hpp"

///Data necessary to launch the getGame()->
struct GameLaunchData
{
	struct PlayerData
	{
		String slaveName;//name of intended slave
		String playerName;
		Money playerMoney;
		String ship;//which ship?
		Team team;//which team are they on
		bool isAI;//is it an AI Player?
	};

	String level;//name of level
	int localController;//which controller the local player has
	List<PlayerData> playerList;
};

