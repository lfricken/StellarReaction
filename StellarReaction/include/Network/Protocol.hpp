#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

enum class Protocol//indicate which type of message it is
{
	/**UDP**/
	Control,//what the player is doing in terms of actions, aim, ect
	Data,//a message regarding the game

	/**TCP**/
	Tcp,
	LoadLevel,//the server wants us to load a game
	Handshake,
	LobbyOption,//switch teams, choose ship

	End,
};

#endif // PROTOCOL_HPP
