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
	Handshake,//client recieves first packet from server
	ReturnHandshake,//server recieves first packet from client
	PlayerOption,//switch teams, choose ship

	End,//end of protocol, or ignore this packet
};

#endif // PROTOCOL_HPP
