#include "Game.hpp"

#include "Globals.hpp"
#include "TextureAllocator.hpp"
#include "AnimAlloc.hpp"
#include "SoundManager.hpp"

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


using namespace std;
using namespace sf;
using namespace leon;


Game::Game()
{
	srand(time(NULL));

	loadWindow("window.ini");



	m_spSound = sptr<SoundManager>(new SoundManager);
	m_spAnimAlloc = sptr<AnimAlloc>(new AnimAlloc);
	m_spCoreIO = sptr<IOManager>(new IOManager(true));

	NetworkBossData nwData;
	m_spNetworkBoss = sptr<NetworkBoss>(new NetworkBoss(nwData));
	IOComponentData overlayData(*m_spCoreIO);
	overlayData.name = "overlay";
	m_spOverlay = sptr<Overlay>(new Overlay(overlayData));
	m_spOverlay->loadMenus();
	PlayerData playerData;
	m_spLocalPlayer = sptr<Player>(new Player(playerData));

	/**== GAME IO COMPONENT ==**/
	IOComponentData gameData(getCoreIO());
	gameData.name = "game";
	m_spIO = sptr<IOComponent>(new IOComponent(gameData, &Game::input, this));

	loadUniverse("RANDOMTEXT");
	m_spUniverse->togglePause(true);
}
Game::~Game()
{
	cout << "\nGame Destroying...";
	cout << "\nExpect to see (0x8000FFFF) upon exit due to SFML audio.";
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
	return m_clock.getElapsedTime().asSeconds();
}
/// <summary>
/// Contains Main Game Loop
/// </summary>
void Game::run()
{
	sf::Listener::setDirection(0, 0, -1);//make sure all sounds are heard with the listener looking at the screen

	{
		std::vector<std::string> controllerList;
		controllerList.push_back("ship_11");
		controllerList.push_back("ship_12");
		controllerList.push_back("ship_13");
		controllerList.push_back("ship_14");
		//getUniverse().loadLevel("levels/level_1/", 0, "blueprints/", controllerList);
	}

	RenderWindow& rWindow = *m_spWindow;
	sf::View defaultView;
	defaultView.setCenter(rWindow.getSize().x/2, rWindow.getSize().y/2);
	defaultView.setSize(sf::Vector2f(rWindow.getSize()));

	float lastTime = 0;
	float frameTime = 0;
	float physTickTimeRemaining = 0;
	float timeStep = getUniverse().getTimeStep();

	while(rWindow.isOpen())
	{
		/**== FRAMERATE ==**/
		frameTime = m_clock.getElapsedTime().asSeconds()-lastTime;
		lastTime = m_clock.getElapsedTime().asSeconds();
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::F))
			std::cout << "\nFPS: " << 1.0f/frameTime;


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
			getUniverse().getControllerFactory().processAllDirectives();

			getUniverse().postPhysUpdate();
			physTickTimeRemaining -= timeStep;
		}


		/**NETWORK**/
		if(m_spNetworkBoss->getNWState() != NWState::Local)
			m_spNetworkBoss->update();


		/**== WINDOW ==**/
		getLocalPlayer().getWindowEvents(rWindow);
		getUniverse().getGfxUpdater().update();


		/**== DRAW UNIVERSE ==**/
		rWindow.clear(sf::Color::Black);
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
}
void Game::exit()
{
	m_spWindow->close();
}





/// <summary>
/// Destroys old universe and makes new one!
/// </summary>
/// <param name="stuff">The stuff.</param>
void Game::loadUniverse(const std::string& stuff)
{
	IOComponentData universeData(getCoreIO());
	universeData.name = "universe";
	m_spUniverse = sptr<Universe>(new Universe(universeData));
	m_spUniverse->getUniverseIO().give(&*m_spIO);
}
/// <summary>
/// Initializes
/// </summary>
/// <param name="windowFile">The window file.</param>
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

	/**LOAD DATA FROM WINDOW**/
	/// if (sf::Shader::isAvailable() && windowData.blurEnabled)
	///    m_shader.loadFromFile(windowData.motionBlurShader, sf::Shader::Fragment);


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
