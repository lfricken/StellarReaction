#include "Globals.hpp"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

Game* getGame(Game* instance)
{
	static Game* inst;

	if(instance == nullptr)
	{
		return inst;
	}
	else
	{
		inst = instance;
		return nullptr;
	}
}

int main()
{
	{
		sptr<Game> game(new Game());
		getGame(game.get());
		//game.initialize();
		//getGame()->run();
	}

	_CrtDumpMemoryLeaks();
	return 0;
}
/*OWNED BY LEON FRICKENSMITH*/
/*
	   ▄▄▀▀▀▀▀▀▀▀▀▀▄▄█▄    ▄    █
	   █▀             ▀▀█▄   ▀         ▄
	   ▄▀                 ▀██   ▄▀▀▀▄▄  ▀
	   ▄█▀▄█▀▀▀▀▄      ▄▀▀█▄ ▀█▄  █▄   ▀█
	   ▄█ ▄▀  ▄▄▄ █   ▄▀▄█▄ ▀█  █▄  ▀█    █
	   ▄█  █   ▀▀▀ █  ▄█ ▀▀▀  █   █▄  █    █
	   ██   ▀▄   ▄█▀   ▀▄▄▄▄▄█▀   ▀█  █▄   █
	   ██     ▀▀▀                  █ ▄█    █
	   ██                     █    ██▀    █▄
	   ██                     █    █       ▀▀█▄
	   ██                    █     █       ▄▄7██
	   ██                  ▄▀     █       ▀▀█▄
	   ▀█      █         ▄█▀      █       ▄▄██
	   ▄██▄     ▀▀▀▄▄▄▄▀▀         █       ▀▀█▄
	   ▀▀▀▀                      █▄▄▄▄▄▄▄▄▄██
	   */
/**
		 ▄              ▄
		 ▌▒█           ▄▀▒▌
		 ▌▒▒█        ▄▀▒▒▒▐
		 ▐▄▀▒▒▀▀▀▀▄▄▄▀▒▒▒▒▒▐
		 ▄▄▀▒ ▒▒▒▒▒▒▒▒▒█▒▒▄█▒▐
		 ▄▀▒▒▒   ▒▒▒   ▒▒▒▀██▀▒▌
		 ▐▒▒▒▄▄▒▒▒▒   ▒▒▒▒▒▒▒▀▄▒▒▌
		 ▌  ▌█▀▒▒▒▒▒▄▀█▄▒▒▒▒▒▒▒█▒▐
		 ▐   ▒▒▒▒▒▒▒▒▌██▀▒▒   ▒▒▒▀▄▌
		 ▌ ▒▄██▄▒▒▒▒▒▒▒▒▒      ▒▒▒▒▌
		 ▀▒▀▐▄█▄█▌▄ ▀▒▒          ▒▒▒▐
		 ▐▒▒▐▀▐▀▒ ▄▄▒▄▒▒▒▒▒▒ ▒ ▒ ▒▒▒▒▌
		 ▐▒▒▒▀▀▄▄▒▒▒▄▒▒▒▒▒▒▒▒ ▒ ▒ ▒▒▐
		 ▌▒▒▒▒▒▒▀▀▀▒▒▒▒▒▒ ▒ ▒ ▒ ▒▒▒▌
		 ▐▒▒▒▒▒▒▒▒▒▒▒▒▒▒ ▒ ▒ ▒▒▄▒▒▐
		 ▀▄▒▒▒▒▒▒▒▒▒▒▒ ▒ ▒ ▒▄▒▒▒▒▌
		 ▀▄▒▒▒▒▒▒▒▒▒▒▄▄▄▀▒▒▒▒▄▀
		 ▀▄▄▄▄▄▄▀▀▀▒▒▒▒▒▄▄▀
		 ▒▒▒▒▒▒▒▒▒▒▀▀
		 **/
