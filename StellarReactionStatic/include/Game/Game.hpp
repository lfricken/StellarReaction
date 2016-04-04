#pragma once

#include "stdafx.hpp"



class Player;
class Universe;
class IOComponent;

class IOManager;
class TextureAllocator;
class AnimAlloc;
class BatchLayers;
class GraphicsComponentUpdater;
class NetworkBoss;
class SoundManager;
class Scoreboard;
class Directory;
struct GameLaunchData;

namespace leon
{
	class Overlay;
	class DragUpdater;
}

/// <summary>
/// Game is global access point for state
/// ultimately holds everything
/// </summary>
class Game
{
public:
	Game();
	virtual ~Game();

	Player& getLocalPlayer();

	float getTime() const;
	sf::RenderWindow& getWindow();
	leon::Overlay& getOverlay();
	IOManager& getCoreIO();
	NetworkBoss& getNwBoss();
	TextureAllocator& getTextureAllocator();
	AnimAlloc& getAnimAlloc();
	SoundManager& getSound();
	leon::DragUpdater& getDragUpdater();
	Scoreboard& getScoreboard();

	Universe& getUniverse();
	const Directory& getDir() const;
	/**should go in UNIVERSE to be reset upon game reload**/	

	void launchGame(const GameLaunchData& data);



	void run();//runs the game on a loop
	void runTicks(int ticks);//run game for a certain number of ticks
	void runTime(float time);//run the game for a certain amount of time

	void exit();

protected:
	void input(std::string rCommand, sf::Packet rData);

private:
	void tick(float frameTime);//increment the game loop one step, telling it that frameTime has passed
	void loadPlayer(const std::string& rFileName);
	void loadWindow(const std::string& windowFile);
	void loadUniverse(const std::string& stuff);

	/// <summary>
	/// IO component for global "game"
	/// </summary>
	sptr<leon::DragUpdater> m_spDragUpdater;
	/// <summary>
	/// The Global IO Manager for the game, which controlls all events (aka not networked or in game related, such as menus).
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
	sptr<leon::Overlay> m_spOverlay;
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
	/// <summary>
	/// Directory handler
	/// </summary>
	sptr<const Directory> m_spDir;

	sptr<Scoreboard> m_spScoreboard;

	const float m_estimatedFrameTime = (1.f / 60.f);

	double m_targetFPS;
	/// <summary>
	/// global clock never paused
	/// </summary>
	sf::Clock m_clock;	
	/// <summary>
	/// hold data to stop calling the clock 1000's of times per 
	/// frame (which takes a lot of cpu time for some reason)
	/// </summary>
	mutable float m_lastTime;
	mutable int m_sampleClock;
	mutable int m_sampleFreq;
};
