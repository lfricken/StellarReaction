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

	void launchGame(const std::string& level, int localController, const std::string& bluePrints, const std::vector<std::string>& rControllerList, const std::vector<std::string>& rShipTitleList);



	void run();//runs the game loop
	void exit();

protected:
	void input(std::string rCommand, sf::Packet rData);

private:
	void loadWindow(const std::string& windowFile);
	void loadUniverse(const std::string& stuff);

	/// <summary>
	/// The Global IO Manager for the game, which controlls all events.
	/// </summary>
	sptr<IOManager> m_spCoreIO;
	/// <summary>
	/// Controls all sound for the game.
	/// </summary>
	sptr<SoundManager> m_spSound;
	/// <summary>
	/// Manages Network Connections.
	/// </summary>
	sptr<NetworkBoss> m_spNetworkBoss;
	/// <summary>
	/// The player on this computer and all info regarding him.
	/// </summary>
	sptr<Player> m_spLocalPlayer;
	/// <summary>
	/// SFML window.
	/// </summary>
	sptr<sf::RenderWindow> m_spWindow;
	/// <summary>
	/// Wrapper for tgui GUI
	/// </summary>
	sptr<Overlay> m_spOverlay;
	/// <summary>
	/// Allocates textures for game.
	/// </summary>
	sptr<TextureAllocator> m_spTexAlloc;
	/// <summary>
	/// Loads and stores animations for QuadComponents
	/// </summary>
	sptr<AnimAlloc> m_spAnimAlloc;
	/// <summary>
	/// Manages physics world
	/// </summary>
	sptr<Universe> m_spUniverse;
	/// <summary>
	/// IO component for global "game"
	/// </summary>
	sptr<IOComponent> m_spIO;

	double m_targetFPS;
	/// <summary>
	/// global clock never paused
	/// </summary>
	sf::Clock m_clock;
};

#endif // GAME_HPP
