#include "Globals.hpp"
#include "Random.hpp"

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>


using namespace std;
Game game;

int main()
{
	Random::seed();
	game.run();

	//_CrtDumpMemoryLeaks;

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
