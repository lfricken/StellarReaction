#pragma once

#include "stdafx.hpp"
#include "NonCopyable.hpp"


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

/// Game is the global access point for data and state.
/// Contains all objects and memory.
/// Built to rely on a single global instance of itself.
class Game : NonCopyable
{
public:
	Game();
	virtual ~Game();

	/// Return reference to local Player.
	Player& getLocalPlayer();
	/// Return current time since Game was initialized.
	float getTime() const;
	/// Return RenderWindow, (the Game Window).
	sf::RenderWindow& getWindow();
	/// Return Overlay.
	leon::Overlay& getOverlay();
	/// Return core IOManager.
	IOManager& getCoreIO();
	/// Return NetworkBoss.
	NetworkBoss& getNwBoss();
	/// Return TextureAllocator.
	TextureAllocator& getTextureAllocator();
	/// Return AnimAlloc (Animation Allocator).
	AnimAlloc& getAnimAlloc();
	/// Return SoundManager.
	SoundManager& getSound();
	/// Return DragUpdater.
	leon::DragUpdater& getDragUpdater();
	/// Return Scoreboard.
	Scoreboard& getScoreboard();
	/// Return SFML View that renders to the standard Window coordinates.
	sf::View& getStaticView();
	/// Return Universe.
	Universe& getUniverse();
	/// Return Directory.
	const Directory& getDir() const;
	
	/// Launch a game with the given GameLaunchData.
	void launchGame(const GameLaunchData& data);
	/// A function to be called by tests to reset the global Game object.
	void restartTest(const std::string& level = "Testbed");
	/// If the Window is resized, we should resize the default view as well.
	void resizeStaticView();

	/// Runs the game on an infinite loop.
	void run();
	/// Run the game for a certain amount of ticks, variable time.
	/// A tick is a single iteration of the game loop.
	void runTicks(int ticks);
	/// Run the game for a certain amount of time, variable ticks.
	void runTime(float time);
	/// Close the game.
	void exit();

protected:
	void input(std::string rCommand, sf::Packet rData);

private:
	void tick(float frameTime);//increment the game loop one step, telling it that frameTime has passed
	void loadPlayer(const std::string& rFileName);
	void loadWindow(const std::string& windowFile);
	void loadUniverse(const std::string& stuff);


	/// Icon for game.
	sptr<sf::Image> m_spIcon;
	/// IO component for global "game"
	sptr<leon::DragUpdater> m_spDragUpdater;
	/// The Global IO Manager for the game, which controlls all events (aka not networked or in game related, such as menus).
	sptr<IOManager> m_spCoreIO;
	/// Controls all sound for the game.
	sptr<SoundManager> m_spSound;
	/// Manages Network Connections.
	sptr<NetworkBoss> m_spNetworkBoss;
	/// The player on this computer and all info regarding them.
	sptr<Player> m_spLocalPlayer;
	/// SFML window.
	sptr<sf::RenderWindow> m_spWindow;
	/// View that does not move. (HUD elements)
	sptr<sf::View> m_spStaticView;
	/// Wrapper for tgui GUI
	sptr<leon::Overlay> m_spOverlay;
	/// Allocates textures for game.
	sptr<TextureAllocator> m_spTexAlloc;
	/// Loads and stores animations for QuadComponents
	sptr<AnimAlloc> m_spAnimAlloc;
	/// Manages physics world
	sptr<Universe> m_spUniverse;
	/// IO component for global "game"
	sptr<IOComponent> m_spIO;	
	/// Directory handler
	sptr<const Directory> m_spDir;

	sptr<Scoreboard> m_spScoreboard;

	const float m_estimatedFrameTime = (1.f / 60.f);

	double m_targetFPS;
	/// Global clock that is never paused.
	sf::Clock m_clock;	

	/// hold data to stop calling the clock 1000's of times per 
	/// frame (which takes a lot of cpu time for some reason)
	mutable float m_lastTime;
	mutable int m_sampleClock;
	mutable int m_sampleFreq;
};
