#pragma once

#include "stdafx.hpp"
#include "Money.hpp"

///Used to launch a game!
struct GameLaunchData
{
	struct PlayerData
	{
		string slaveName;//name of intended slave
		string playerName;
		Money playerMoney;
		string ship;//which ship?
		int team;//which team are they on
		bool isAI;//is it an AI Player?
	};

	string level;//name of level
	int localController;//which controller the local player has
	vector<PlayerData> playerList;
};

