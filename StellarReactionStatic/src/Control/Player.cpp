#include "Player.hpp"
#include "Globals.hpp"
#include "Overlay.hpp"
#include "Panel.hpp"
#include "Universe.hpp"
#include "IOManager.hpp"
#include "Convert.hpp"
#include "LinearMeter.hpp"
#include "Minimap.hpp"
#include "Chunk.hpp"
#include "CommandInfo.hpp"

using namespace std;
using namespace sf;

Player::Player(const PlayerData& rData) : m_io(rData.ioComp, &Player::input, this), BasePlayerTraits(rData.name)
{
	m_hasFocus = true;
	m_inGuiMode = true;
	m_tracking = rData.tracking;
	for(int i = 1; i <= 9; ++i)
	{
		m_weaponGroups[i] = true;
	}
}
Player::~Player()
{
	cout << "\nPlayer Destroying...";
}
Camera& Player::getCamera()
{
	return m_camera;
}
b2Vec2 Player::getMouseInWorld()
{
	return m_aim;
}
const InputConfig& Player::getInCfg() const
{
	return m_inCfg;
}
/// <summary>
/// Are the player inputs going to the gui or the controller
/// </summary>
bool Player::inGuiMode() const//is the player in GUI mode?
{
	return m_inGuiMode;
}
bool Player::toggleGuiMode(bool isGuiModeOn)
{
	return m_inGuiMode = isGuiModeOn;
}
/// <summary>
/// Is our camera in tracking mode?
/// </summary>
bool Player::isTracking() const
{
	return m_tracking;
}
int Player::radarsize()
{
	return m_radarsize;
}
const sf::Vector2f& Player::getMouseWindowPos() const
{
	return m_mouseWindowPos;
}
void Player::setMouseWindowPos(const sf::Vector2f& rPos)
{
	sf::Mouse::setPosition(sf::Vector2i((int)rPos.x, (int)rPos.y), game.getWindow());
	m_mouseWindowPos = rPos;
}
/// <summary>
/// Indicate which controller our inputs should modify
/// </summary>
void Player::setController(int index)
{
	game.getUniverse().getControllerFactory().unsetLocal();

	m_controller = index;
	Controller& rController2 = game.getUniverse().getControllerFactory().getController(m_controller);
	rController2.toggleLocal(true);
}
/// <summary>
/// Get's the state of keyboard and mouse at this exact moment.
/// </summary>
void Player::getLiveInput()
{
	Controller& rController = game.getUniverse().getControllerFactory().getController(m_controller);

	//default to false
	for(auto it = m_directives.begin(); it != m_directives.end(); ++it)
		it->second = false;

	if(!m_inGuiMode && hasFocus())
	{
		/**== CAMERA ==**/
		const float speed = 0.05f;
		if(Keyboard::isKeyPressed(m_inCfg.cameraUp))
			m_camera.move(b2Vec2(0, speed));
		if(Keyboard::isKeyPressed(m_inCfg.cameraDown))
			m_camera.move(b2Vec2(0, -speed));
		if(Keyboard::isKeyPressed(m_inCfg.cameraLeft))
			m_camera.move(b2Vec2(-speed, 0));
		if(Keyboard::isKeyPressed(m_inCfg.cameraRight))
			m_camera.move(b2Vec2(speed, 0));


		/**== KEYBOARD ==**/
		if(Keyboard::isKeyPressed(m_inCfg.up))
			m_directives[Directive::Up] = true;
		if(Keyboard::isKeyPressed(m_inCfg.down))
			m_directives[Directive::Down] = true;
		if(Keyboard::isKeyPressed(m_inCfg.rollCCW))
			m_directives[Directive::RollCCW] = true;
		if(Keyboard::isKeyPressed(m_inCfg.rollCW))
			m_directives[Directive::RollCW] = true;
		if(Keyboard::isKeyPressed(m_inCfg.boost))
			m_directives[Directive::Boost] = true;
		if(Keyboard::isKeyPressed(m_inCfg.stealth))
			m_directives[Directive::Stealth] = true;
		if(Keyboard::isKeyPressed(m_inCfg.teleport))
			m_directives[Directive::Teleport] = true;

		if(Keyboard::isKeyPressed(m_inCfg.shield))
			m_directives[Directive::Shield] = true;

		/**== SPECIAL ==**/
		if(Keyboard::isKeyPressed(m_inCfg.store))
			m_directives[Directive::ShowStore] = true;
		if(Keyboard::isKeyPressed(m_inCfg.respawn))
			m_directives[Directive::Respawn] = true;


		/**== MOUSE ==**/
		if(Mouse::isButtonPressed(m_inCfg.primary))
			m_directives[Directive::FirePrimary] = true;
		if(Mouse::isButtonPressed(m_inCfg.secondary))
			m_directives[Directive::FireSecondary] = true;

		m_aim = leon::sfTob2(game.getWindow().mapPixelToCoords(Mouse::getPosition(game.getWindow()), m_camera.getView()));

		/**== DEVELOPER ==**/
		if(Keyboard::isKeyPressed(Keyboard::Numpad8))
			cout << "\n(" << m_aim.x << ",\t" << m_aim.y << ")";

		Controller& rController = game.getUniverse().getControllerFactory().getController(m_controller);

	}

	m_mouseWindowPos = game.getWindow().mapPixelToCoords(Mouse::getPosition(game.getWindow()), game.getStaticView());

	CommandInfo commands;
	commands.directives = m_directives;
	commands.weaponGroups = m_weaponGroups;
	commands.isLocal = true;
	rController.locallyUpdate(commands);
	rController.setAim(m_aim);
}
/// <summary>
/// Handle window events such as clicks and gui events.
/// </summary>
void Player::getWindowEvents(sf::RenderWindow& rWindow)//process window events
{
	sf::Event event;
	Controller& rController = game.getUniverse().getControllerFactory().getController(m_controller);

	while(rWindow.pollEvent(event))
	{
		if(event.type == sf::Event::LostFocus)
			toggleFocus(false);
		if(event.type == sf::Event::GainedFocus)
			toggleFocus(true);
		if(event.type == sf::Event::Resized)
		{
			//TODO WE MAY NEED TO ADJUST OTHER VIEW ELEMENTS???
			game.resizeStaticView();
			m_camera.resize();
			game.getOverlay().getGui().handleEvent(event, false);
		}

		/** CLOSE **/
		if(event.type == sf::Event::Closed)
			rWindow.close();
		if(event.type == Event::KeyPressed)
		{
			/**== MAIN MENU ==**/
			if(event.key.code == Keyboard::Escape)
			{
				Message menu("overlay", "toggleMenu", voidPacket, 0, false);
				game.getCoreIO().recieve(menu);
			}
			/**== SCOREBOARD ==**/
			if(event.key.code == Keyboard::Tab)
			{
				Message scoreboard("overlay", "toggleScoreboard", voidPacket, 0, false);
				game.getCoreIO().recieve(scoreboard);
				//create message to display scoreboard overlay
				//receive message
			}
		}


		/**== GUI ==**/
		if(m_inGuiMode)
			game.getOverlay().handleEvent(event);
		else
		{
			/**== ZOOM ==**/
			if(event.type == sf::Event::MouseWheelMoved)
			{
				int change = -event.mouseWheel.delta;
				if(change < 0)
					m_camera.setZoom(m_camera.getZoom()*0.8f);
				else
					m_camera.setZoom(m_camera.getZoom()*1.2f);
			}
			b2Body* pBody = rController.getBodyPtr();//make sure we arent over zoomed!
			if(pBody != NULL)
			{
				float zoomValue = rController.get(Request::Zoom);
				float x1 = (float)game.getWindow().getSize().x; //getting the siz of x
				float y1 = (float)game.getWindow().getSize().y; //getting the size of y
				float area = 1920 * 1080; // setting a fixed resolution
				float area1 = x1*y1;

				float newzoomvalue = zoomValue*sqrt(area / area1); // setting a new zoom value based on the current screen's resolution

				if(newzoomvalue < m_camera.getZoom())
					m_camera.setZoom(newzoomvalue);
			}



			if(event.type == Event::KeyPressed)
			{
				/**== Press Event ==**/
				if(event.key.code == m_inCfg.cgroup_1)
					toggleControlGroup(1);
				if(event.key.code == m_inCfg.cgroup_2)
					toggleControlGroup(2);
				if(event.key.code == m_inCfg.cgroup_3)
					toggleControlGroup(3);
				if(event.key.code == m_inCfg.cgroup_4)
					toggleControlGroup(4);


				/**== DEVELOPER OPTIONS ==**/
				if(event.key.code == Keyboard::Numpad9)
					game.getUniverse().toggleDebugDraw();
				if(event.key.code == Keyboard::Numpad5)
					m_tracking = !m_tracking;
				if(event.key.code == Keyboard::Numpad0)
					game.getUniverse().togglePause();
			}
		}
	}
}
void Player::updateView()
{
	Controller& rController = game.getUniverse().getControllerFactory().getController(m_controller);
	b2Body* pBody = rController.getBodyPtr();
	if(!m_inGuiMode && pBody != NULL)
	{
		if(m_tracking)
			m_camera.setPosition(pBody->GetPosition());

		//Energy Bar
		float val = rController.get(Request::Energy);
		float maxVal = rController.get(Request::MaxEnergy);
		if(maxVal <= 0.f)
		{
			maxVal = 1.f;
			val = 0.f;
		}
		m_energyMeterFill->setPercent(val / maxVal);

		//Energy Warning
		if(val / maxVal < 0.1f)
			m_energyDanger->getAnimator().setAnimation("Default", 2.f);

		//Out of Bounds
		b2Vec2 location = pBody->GetPosition();
		b2Vec2 bounds = game.getUniverse().getBounds();
		if(abs(location.x) > bounds.x || abs(location.y) > bounds.y)//if out of bounds
			m_boundsDanger->getAnimator().setAnimation("Default", 2.f);


		//Score and Money
		int score = (int)rController.get(Request::Score);
		static int oldScore = -1;
		string scoreString = "Score: ";
		string moneyString = "Money: " + to_string(getMoney());

		if(score != oldScore)
		{
			oldScore = score;
			scoreString += to_string(oldScore);

			if(oldScore == 10)
			{
				// TODO this should not have a win condition
				scoreString = "You Win";
				game.getUniverse().togglePause();
			}

			Packet scorePack;
			scorePack << scoreString;
			Message setScore("hud_score", "setText", scorePack, 0, false);
			game.getCoreIO().recieve(setScore);
		}
		Packet moneyPack;
		moneyPack << moneyString;
		Message setMoney("hud_money", "setText", moneyPack, 0, false);
		game.getCoreIO().recieve(setMoney);


		//Radar TODO THIS ALL SHOULD BE IN THE MINIMAP CLASS
		std::vector<sptr<GameObject> > goList = game.getUniverse().getgoList();
		int index = 0;
		float maxRange = 50.f;
		float mapScale = -0.005f;
		const static Vec2 miniMapCenter = m_minimap->getPosition();
		m_radarsize = goList.size();
		for(auto it = goList.begin(); it != goList.end(); ++it)
		{
			//Determine team.
			GameObject* p = it->get();
			Chunk* object = dynamic_cast<Chunk*>(p);
			Team other_team = object->getBodyComponent().getTeam();
			
			sf::Color dotColor(sf::Color::Blue);

			if(other_team == Team::Neutral)
				dotColor = sf::Color::Blue;
			else if(other_team == Team::Capturable)
				dotColor = sf::Color(255, 140, 0);
			else if(other_team == Team::Invalid)
				dotColor = sf::Color::Magenta;
			else if(other_team == this->getTeam())
				dotColor = sf::Color::Green;
			else
				dotColor = sf::Color::Red;

			if(object != NULL && !object->isStealth())
			{
				b2Vec2 dif = pBody->GetPosition() - object->getBodyPtr()->GetPosition();
				float dist = dif.Length();

				if(dist < maxRange)// TODO fix magic number horror
				{

					dif *= mapScale;
					m_minimap->setDot(miniMapCenter + dif, index, dotColor);
					index++;
				}
			}
		}
		m_minimap->cleanMap(index);
	}
}
IOComponent& Player::getIOComp()
{
	return m_io;
}
QuadComponentData createUI(sf::Vector2f size, std::string displayName, sf::Vector2f center = sf::Vector2f())
{
	QuadComponentData data;
	data.dimensions = size;
	data.texName = displayName + ".png";
	data.animSheetName = displayName + ".acfg";
	data.layer = GraphicsLayer::OverlayBottom;

	if(center != sf::Vector2f())
		data.center = center;

	return data;
}
void Player::loadOverlay(const std::string& rOverlay)
{
	b2Vec2 emeterPos = b2Vec2(0.05f, -0.05f);
	Vec2 winDim(leon::sfTob2((sf::Vector2f)game.getWindow().getSize()));

	//Thing covering fill
	LinearMeterData fillData;
	fillData.dimensions = sf::Vector2f(30, 124);
	fillData.layer = GraphicsLayer::OverlayMiddle;
	fillData.center = sf::Vector2f(-fillData.dimensions.x / 2, fillData.dimensions.y / 2);
	m_energyMeterFill.reset(new LinearMeter(fillData));
	m_energyMeterFill->setPosition(emeterPos);

	//Mini Map
	MinimapData mapData;
	float dims = 256;
	mapData.animSheetName = "overlay/white.acfg";
	mapData.texName = "overlay/white.png";
	mapData.color = sf::Color(96, 96, 96, 32);
	mapData.dimensions = sf::Vector2f(dims, dims);
	mapData.layer = GraphicsLayer::OverlayMiddle;
	m_minimap.reset(new Minimap(mapData));
	m_minimap->setPosition(Vec2(winDim.x - (dims*0.5 / scale), winDim.y + (dims*0.5 / scale)));

	//Energy Bar
	sf::Vector2f dimensions = sf::Vector2f(32, 128);
	QuadComponentData data = createUI(dimensions, "overlay/meter", sf::Vector2f(-dimensions.x / 2, dimensions.y / 2));
	m_energyMeter.reset(new QuadComponent(data));
	m_energyMeter->setPosition(emeterPos);

	//Energy Warning
	QuadComponentData datawarn = createUI(sf::Vector2f(86, 74), "overlay/warning_energy");
	m_energyDanger.reset(new QuadComponent(datawarn));
	m_energyDanger->setPosition(emeterPos + b2Vec2(0.05f, -0.4f));

	//Out of Bounds Warning
	QuadComponentData dataWarnBounds = createUI(sf::Vector2f(250, 73), "overlay/warning_bounds");
	m_boundsDanger.reset(new QuadComponent(dataWarnBounds));
	m_boundsDanger->setPosition(b2Vec2(1.35f, -0.3f));



	// Create a group icon for each possible group.
	for(int group = 0; group < 4; ++group)
	{
		// Create one for each group.
		QuadComponentData groupData = createUI(sf::Vector2f(100, 50), "overlay/control_group");

		// Generate a new sptr to grouping icon.
		sptr<QuadComponent> groupIcon;
		groupIcon.reset(new QuadComponent(groupData));
		groupIcon->setPosition(b2Vec2(1 + 0.2*group, 0) + emeterPos);

		m_groupIcon.push_back(groupIcon);
	}

}
void Player::universeDestroyed()
{
	m_energyMeter.reset();
	m_energyMeterFill.reset();
	m_energyDanger.reset();
	m_minimap.reset();
	m_boundsDanger.reset();

	// Clear, because otherwise, when we go 
	// to add 4 more to it, the old null
	// pointers are left behind and cause a crash.
	m_groupIcon.clear();
}
bool Player::toggleFocus(bool isWindowFocused)
{
	m_hasFocus = isWindowFocused;
	return m_hasFocus;
}
bool Player::hasFocus() const
{
	return m_hasFocus;
}

bool Player::toggleControlGroup(int group, bool on)
{
	// HUD Icons.
	int index = group - 1;
	if(index < m_groupIcon.size())
	{
		if(on)
			m_groupIcon[index]->getAnimator().setAnimation("On", 1);
		else
			m_groupIcon[index]->getAnimator().setAnimation("Off", 1);
	}

	return m_weaponGroups[group] = on;
}
bool Player::toggleControlGroup(int group)
{
	return toggleControlGroup(group, !m_weaponGroups[group]);
}
void Player::input(std::string rCommand, sf::Packet rData)
{
	sf::Packet data(rData);
	if(rCommand == "toggleGuiMode")
	{
		toggleGuiMode(!m_inGuiMode);
	}
	else if(rCommand == "setGuiMode")
	{
		bool mode = false;
		data >> mode;
		toggleGuiMode(mode);
	}
}