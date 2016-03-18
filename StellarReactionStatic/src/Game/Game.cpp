#include "Game.hpp"

#include "Globals.hpp"
#include "TextureAllocator.hpp"
#include "AnimAlloc.hpp"
#include "SoundManager.hpp"
#include "DragUpdater.hpp"

#include "BatchLayers.hpp"
#include "GraphicsComponentUpdater.hpp"
#include "Overlay.hpp"
#include "IOManager.hpp"
#include "Player.hpp"
#include "Universe.hpp"
#include "NetworkBoss.hpp"
#include "Convert.hpp"
#include "SlaveLocator.hpp"
#include "Chunk.hpp"
#include "QuadComponent.hpp"
#include "Spinner.hpp"
#include "LinearMeter.hpp"
#include "DecorQuad.hpp"
#include "Beam.hpp"
#include "RayCastCallback.hpp"
#include "Projectile.hpp"
#include "Directory.hpp"

using namespace std;
using namespace sf;
using namespace leon;


Game::Game()
{
	srand(static_cast<unsigned int>(time(NULL)));

	m_spDragUpdater = sptr<DragUpdater>(new DragUpdater());

	loadWindow(contentDir() + "window.ini");

	m_sampleClock = 0;
	m_sampleFreq = 40;
	m_lastTime = 0;

	m_spSound = sptr<SoundManager>(new SoundManager);
	sf::Listener::setDirection(0, 0, -1);//make sure all sounds are heard with the listener looking at the screen
	m_spAnimAlloc = sptr<AnimAlloc>(new AnimAlloc);
	m_spCoreIO = sptr<IOManager>(new IOManager(true));

	NetworkBossData nwData;
	m_spNetworkBoss = sptr<NetworkBoss>(new NetworkBoss(nwData));
	IOComponentData overlayData(*m_spCoreIO);
	overlayData.name = "overlay";
	m_spOverlay = sptr<Overlay>(new Overlay(overlayData));
	m_spOverlay->loadMenus();

	loadPlayer(contentDir() + "settings/GeneralSettings.cfg");

	/**== GAME IO COMPONENT ==**/
	IOComponentData gameData(getCoreIO());
	gameData.name = "game";
	m_spIO = sptr<IOComponent>(new IOComponent(gameData, &Game::input, this));

	loadUniverse("RANDOMTEXT");//TODO RANDOMTEXT
	m_spUniverse->togglePause(true);

	m_spDir = sptr<Directory>(new Directory("../"));

	ScoreboardData scoreData  = ScoreboardData();
	m_spScoreboard = sptr<Scoreboard>(new Scoreboard(scoreData));
}
Game::~Game()
{
	cout << "\nGame Destroying...";
	cout << "\nExpect to see (0x8000FFFF) upon exit due to SFML audio.";
}
void Game::loadPlayer(const std::string& rFileName)
{
	PlayerData data;

	Json::Value root;//Let's parse it
	Json::Reader reader;
	std::ifstream test(rFileName, std::ifstream::binary);
	bool parsedSuccess = reader.parse(test, root, false);

	if(!parsedSuccess)
	{
		std::cout << "\nFailed to parse JSON file [" << rFileName << "]." << std::endl << FILELINE;
		///eRROR LOG
	}
	else
	{
		data.name = root["PlayerName"].asString();
	}

	m_spLocalPlayer = sptr<Player>(new Player(data));
}
Player& Game::getLocalPlayer()
{
	return *m_spLocalPlayer;
}
sf::RenderWindow& Game::getWindow()
{
	return *m_spWindow;
}
Overlay& Game::getOverlay()
{
	return *m_spOverlay;
}
IOManager& Game::getCoreIO()
{
	return *m_spCoreIO;
}
NetworkBoss& Game::getNwBoss()
{
	return *m_spNetworkBoss;
}
TextureAllocator& Game::getTextureAllocator()
{
	return *m_spTexAlloc;
}
AnimAlloc& Game::getAnimAlloc()
{
	return *m_spAnimAlloc;
}
Universe& Game::getUniverse()
{
	return *m_spUniverse;
}
DragUpdater& Game::getDragUpdater()
{
	return *m_spDragUpdater;
}
const Directory& Game::getDir() const
{
	return *m_spDir;
}
Scoreboard& Game::getScoreboard()
{
	return *m_spScoreboard;
}
/// <summary>
/// Use this to put the player in a new level from anywhere (multiplayer or otherwise)
/// It will minimize the menu's automatically
/// </summary>
/// <param name="level">The level.</param>
/// <param name="localController">The local controller.</param>
/// <param name="bluePrints">The blue prints.</param>
/// <param name="rControllerList">The r controller list.</param>
void Game::launchGame(const std::string& level, int localController, const std::string& bluePrints, const std::vector<std::string>& rControllerList, const std::vector<std::string>& rShipTitleList, const std::vector<int>& teams)
{
	game.loadUniverse("meaninglessString");
	game.getUniverse().loadLevel(level, localController, bluePrints, rControllerList, rShipTitleList, teams);

	sf::Packet boolean;
	boolean << false;

	Message closeMenu("overlay", "setMenu", boolean, 0, false);
	game.getCoreIO().recieve(closeMenu);
}
SoundManager& Game::getSound()
{
	return *m_spSound;
}
/// <summary>
/// Global Time, cannot be paused
/// </summary>
/// <returns></returns>
float Game::getTime() const
{
	if(m_sampleClock < m_sampleFreq)
		++m_sampleClock;
	else
	{
		m_sampleClock = 0;
		m_lastTime = m_clock.getElapsedTime().asSeconds();
	}
	return m_lastTime;

}
void Game::runTicks(int ticks)
{
	if(game.getUniverse().isPaused())
		game.getUniverse().togglePause();

	RenderWindow& rWindow = *m_spWindow;
	float frameTime = 0;
	float lastTime = m_clock.getElapsedTime().asSeconds() - m_estimatedFrameTime;

	for(; ticks > 0; --ticks)
	{
		frameTime = m_clock.getElapsedTime().asSeconds() - lastTime;
		lastTime = m_clock.getElapsedTime().asSeconds();
		tick(frameTime);
	}

	if(!game.getUniverse().isPaused())
		game.getUniverse().togglePause();
}
void Game::runTime(float time)
{
	if(game.getUniverse().isPaused())
		game.getUniverse().togglePause();

	RenderWindow& rWindow = *m_spWindow;
	float frameTime = 0;
	float lastTime = m_clock.getElapsedTime().asSeconds() - m_estimatedFrameTime;

	for(; time > 0; time -= frameTime)
	{
		frameTime = m_clock.getElapsedTime().asSeconds() - lastTime;
		lastTime = m_clock.getElapsedTime().asSeconds();
		tick(frameTime);
	}

	if(!game.getUniverse().isPaused())
		game.getUniverse().togglePause();
}
/// <summary>
/// Contains Main Game Loop
/// </summary>
void Game::run()
{
	RenderWindow& rWindow = *m_spWindow;
	float frameTime = 0;
	float lastTime = m_clock.getElapsedTime().asSeconds();

	while(rWindow.isOpen())
	{
		frameTime = m_clock.getElapsedTime().asSeconds() - lastTime;
		lastTime = m_clock.getElapsedTime().asSeconds();
		tick(frameTime);
	}
}
void Game::tick(float frameTime)
{
	RenderWindow& rWindow = *m_spWindow;

	static float physTickTimeRemaining = 0;
	static const float timeStep = getUniverse().getTimeStep();
	static const sf::View defaultView(Vector2f((float)(rWindow.getSize().x / 2), (float)(rWindow.getSize().y / 2)), Vector2f((float)rWindow.getSize().x, (float)rWindow.getSize().y));


	/**== FRAMERATE ==**/
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::F))
		std::cout << "\nFPS: " << 1 / frameTime;
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::M))
		std::cout << "\nM: " << game.getLocalPlayer().getMoney();

	/**== IO ==**/
	getCoreIO().update(frameTime);
	getUniverse().getUniverseIO().update(frameTime);


	/**== PHYSICS ==**/
	physTickTimeRemaining += frameTime;
	while(physTickTimeRemaining >= timeStep)
	{
		getUniverse().prePhysUpdate();
		getUniverse().physUpdate();

		getLocalPlayer().updateView();
		rWindow.setView(getLocalPlayer().getCamera().getView());
		getLocalPlayer().getLiveInput();
		getUniverse().updateShipAI();
		getUniverse().getControllerFactory().processAllDirectives();

		getUniverse().postPhysUpdate();
		physTickTimeRemaining -= timeStep;
	}

	/**REWARDS**/
	getUniverse().teamMoneyUpdate();

	/**NETWORK**/
	if(m_spNetworkBoss->getNWState() != NWState::Local)
		m_spNetworkBoss->update();


	/**== WINDOW ==**/
	getDragUpdater().update(getLocalPlayer().getMouseWindowPos());
	rWindow.setView(getLocalPlayer().getCamera().getView());
	getLocalPlayer().getWindowEvents(rWindow);
	getUniverse().updateDecorationPosition(getLocalPlayer().getCamera().getPosition(), getLocalPlayer().getCamera().getZoom());
	getUniverse().getGfxUpdater().update();//update graphics components



	/**== DRAW UNIVERSE ==**/
	rWindow.clear(sf::Color::Black);

	rWindow.setView(defaultView);
	getUniverse().getBatchLayers().drawBackground(rWindow);

	rWindow.setView(getLocalPlayer().getCamera().getView());
	if(getUniverse().debugDraw())
		getUniverse().getWorld().DrawDebugData();
	else
		getUniverse().getBatchLayers().drawWorld(rWindow);


	/**== DRAW GUI/OVERLAYS ==**/
	rWindow.setView(defaultView);
	getUniverse().getBatchLayers().drawOverlay(rWindow);
	m_spOverlay->getGui().draw();

	/**== DISPLAY ==**/
	rWindow.display();
}
/// <summary>
/// Literally exits the game.
/// </summary>
void Game::exit()
{
	m_spWindow->close();
}
/// <summary>
/// Destroys old universe and makes new one!
/// </summary>
void Game::loadUniverse(const std::string& stuff)
{
	IOComponentData universeData(getCoreIO());
	universeData.name = "universe";
	m_spUniverse.reset();
	m_spUniverse = sptr<Universe>(new Universe(universeData));
	m_spUniverse->getUniverseIO().give(&*m_spIO);
	if(game.getNwBoss().getNWState() == NWState::Client)
		getUniverse().getUniverseIO().toggleAcceptsLocal(false);
	else
		getUniverse().getUniverseIO().toggleAcceptsLocal(true);
}
/// <summary>
/// Initializes the window from a json file with the needed data.
/// </summary>
void Game::loadWindow(const std::string& windowFile)
{
	sf::ContextSettings settings;
	struct WindowInitData
	{
		WindowInitData()
		{
			windowName = "FIXME";
			windowMode = "windowed";
			mode = sf::VideoMode(640, 640, 32);
			antiAliasLevel = 0;
			smoothTextures = false;
			blurEnabled = false;
			vSinc = false;
			targetFPS = 10;
		}
		std::string windowName;//name of window to display
		std::string defaultFont;//font file
		bool windowMode;//windowed vs fullscreen
		sf::VideoMode mode;
		int antiAliasLevel;
		bool smoothTextures;
		std::string motionBlurShader;
		bool blurEnabled;
		bool vSinc;
		int targetFPS;
	};
	WindowInitData windowData;


	Json::Value root;//Let's parse it
	Json::Reader reader;
	std::ifstream test(windowFile, std::ifstream::binary);
	bool parsedSuccess = reader.parse(test, root, false);

	if(!parsedSuccess)
	{
		std::cout << "\nFailed to parse JSON file [" << windowFile << "]." << std::endl << FILELINE;
		///eRROR LOG
	}
	else
	{
		windowData.windowName = root["windowName"].asString();
		windowData.defaultFont = root["defaultFont"].asString();
		windowData.windowMode = root["windowed"].asBool();
		windowData.mode = sf::VideoMode(root["resX"].asInt(), root["resY"].asInt(), root["color"].asInt());
		windowData.antiAliasLevel = root["antiAliasLevel"].asInt();
		windowData.smoothTextures = root["smoothTextures"].asBool();
		windowData.motionBlurShader = root["motionBlurFile"].asString();
		windowData.blurEnabled = root["motionBlurEnabled"].asBool();
		windowData.vSinc = root["vSinc"].asBool();
		windowData.targetFPS = root["targetFPS"].asInt();
	}


	settings.antialiasingLevel = windowData.antiAliasLevel;
	int style;//the sf::style enum has no name!!
	if(windowData.windowMode)//windowed or fullscreen?
	{
		style = sf::Style::Default;
	}
	else
	{
		style = sf::Style::Fullscreen;
	}


	/**CREATE THE WINDOW AND TEXTURE ALLOC**/
	if(m_spWindow)//if we are already pointing at something
	{
		///close the old window???
		m_spWindow->create(windowData.mode, windowData.windowName, style, settings);
		m_spTexAlloc->smoothTextures(windowData.smoothTextures);
	}
	else//if this is the first time we created something
	{
		m_spWindow.reset(new sf::RenderWindow(windowData.mode, windowData.windowName, style, settings));
		m_spTexAlloc.reset(new TextureAllocator(windowData.smoothTextures));
	}

	m_spWindow->setVerticalSyncEnabled(windowData.vSinc);
	cout << "\nFPS Limit:" << windowData.targetFPS;
	m_spWindow->setFramerateLimit(windowData.targetFPS);
}
/// <summary>
/// process the command
/// </summary>
/// <param name="rCommand">The r command.</param>
/// <param name="rData">The r data.</param>
void Game::input(std::string rCommand, sf::Packet rData)
{
	if(rCommand == "exit")
	{
		exit();
	}
	else if(rCommand == "load")
	{
		///TODO
	}
	else if(rCommand == "printToConsole")
	{
		string str;
		rData >> str;
		cout << "\n" << str;
	}
	else
	{
		cout << "Game: [" << rCommand << "] not found.";
	}
}
