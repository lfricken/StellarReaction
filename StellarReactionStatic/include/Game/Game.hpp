#pragma once

#include "stdafx.hpp"
#include "NonCopyable.hpp"
#include "Clock.hpp"


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

/// \brief Game is the global access point for data and state.
///
/// Contains all objects and memory.
/// Built to rely on a single global instance of itself.
class Game : public Clock, NonCopyable
{
public:
	Game();
	void initialize();
	virtual ~Game();

	/// Return reference to local Player.
	Player& getLocalPlayer();
	/// <summary>
	/// Return time since Game was initialized.
	/// </summary>
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
	
	/// <summary>
	/// Use this to put the player in a new level from anywhere (multiplayer or otherwise).
	/// It will minimize the menu's automatically.
	/// </summary>
	void launchGame(const GameLaunchData& data);
	/// A function to be called by tests to reset the global Game object.
	void restartTest(const String& level = "Testbed");
	/// If the Window is resized, we should resize the default view as well.
	void resizeStaticView();

	/// Runs the game on an infinite loop.
	void run();
	/// Run the game for a certain amount of ticks, variable time.
	/// A tick is a single iteration of the game loop.
	void runTicks(int ticks);
	/// Run the game for a certain amount of time, variable ticks.
	void runTime(float time);
	/// Close the getGame()->
	void exit(); 
	/// <summary>
	/// update internal time
	/// </summary>
	void updateTime();

	void* getClassRegister();

protected:
	void input(String rCommand, sf::Packet rData);

private:
	/// <summary>
	/// Register classes.
	/// </summary>
	void registerClasses();
	sptr<void> classRegister;
	/// Increment the game loop one step, telling it that frameTime seconds have passed.
	void tick(float frameTime);
	/// Loads player data from a file.
	void loadPlayer(const String& rFileName);
	/// Load window with specified settings in a file.
	void loadWindow(const String& windowFile);
	/// Load the universe. Input does nothing at the moment.
	void loadUniverse(const String& stuff);

	/// Icon for getGame()->
	sptr<sf::Image> m_spIcon;
	/// IO component for global "game"
	sptr<leon::DragUpdater> m_spDragUpdater;
	/// The Global IO Manager for the game, which controlls all events (aka not networked or in game related, such as menus).
	sptr<IOManager> m_spCoreIO;
	/// Controls all sound for the getGame()->
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
	/// Allocates textures for getGame()->
	sptr<TextureAllocator> m_spTexAlloc;
	/// Loads and stores animations for QuadComponents
	sptr<AnimAlloc> m_spAnimAlloc;
	/// Manages physics world
	sptr<Universe> m_spUniverse;
	/// IO component for global "game"
	sptr<IOComponent> m_spIO;	
	/// Directory handler
	sptr<const Directory> m_spDir;
	/// Score board. TODO this should be in Universe.
	sptr<Scoreboard> m_spScoreboard;
	/// Used for a few calculations.
	const float m_estimatedFrameTime = (1.f / 60.f);
	/// Places limit on max FPS.
	double m_targetFPS;
	/// Global clock that is never paused.
	sf::Clock m_clock;	

	/// hold data to stop calling the clock 1000's of times per 
	/// frame (which takes a lot of cpu time for some reason)
	mutable float m_lastTime;
	mutable int m_sampleClock;
	mutable int m_sampleFreq;
};
