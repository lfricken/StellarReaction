#pragma once

/// Used to determine a data packets intentional function.
enum class Protocol
{
	/**UDP**/
	Control,//what the player is doing in terms of actions, aim, ect
	Data,//a message regarding the game state like module health or ship position

	/**TCP**/
	Tcp,
	LoadLevel,//the server wants us to load a game
	Handshake,//client recieves first packet from server
	ReturnHandshake,//server recieves first packet from client
	PlayerOption,//switch teams, choose ship
	PlayerTraits,//values in the player traits
	SpecialIoEvent,//an io message should be sent to 1 person

	End,//end of protocol, or ignore this packet
};

