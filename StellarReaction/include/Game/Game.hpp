#ifndef GAME_HPP
#define GAME_HPP

#include "stdafx.hpp"

class Player;
class Universe;
class IOComponent;

class Overlay;
class IOManager;
class TextureAllocator;
class AnimAlloc;
class BatchLayers;
class GraphicsComponentUpdater;
class NetworkBoss;
class SoundManager;


class Game
{
public:
	Game();
	virtual ~Game();

	Player& getLocalPlayer();

	float getTime() const;
	sf::RenderWindow& getWindow();
	Overlay& getOverlay();
	IOManager& getCoreIO();
	NetworkBoss& getNwBoss();
	TextureAllocator& getTextureAllocator();
	AnimAlloc& getAnimAlloc();
	SoundManager& getSound();


	Universe& getUniverse();
	/**should go in UNIVERSE to be reset upon game reload**/

	/**should go in UNIVERSE to be reset upon game reload**/


	void loadWindow(const std::string& windowFile);//loads the window
	void loadUniverse(const std::string& stuff);
	void run();//runs the game loop
	void exit();

protected:
	void input(std::string rCommand, sf::Packet rData);

private:
	sptr<IOManager> m_spCoreIO;//manages IO just for the GUI
	sptr<SoundManager> m_spSound;

	sptr<NetworkBoss> m_spNetworkBoss;
	sptr<Player> m_spLocalPlayer;


	sptr<sf::RenderWindow> m_spWindow;
	sptr<Overlay> m_spOverlay;

	sptr<TextureAllocator> m_spTexAlloc;
	sptr<AnimAlloc> m_spAnimAlloc;


	sptr<Universe> m_spUniverse;

	sptr<IOComponent> m_spIO;

	double m_targetFPS;
	sf::Clock m_clock;
};

#endif // GAME_HPP
