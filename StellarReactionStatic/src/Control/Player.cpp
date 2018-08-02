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
#include "Particles.hpp"
#include "SoundManager.hpp"
#include "Convert.hpp"
#include "Decoration.hpp"
#include "Debugging.hpp"
#include "Grid.hpp"

InputConfig::InputConfig()
{
	/**MOVEMENT**/
	up = sf::Keyboard::W;
	down = sf::Keyboard::S;
	rollCCW = sf::Keyboard::A;
	rollCW = sf::Keyboard::D;

	/**SPECIAL**/
	stealth = sf::Keyboard::Q;
	shield = sf::Keyboard::Space;
	teleport = sf::Keyboard::E;
	boost = sf::Keyboard::LShift;

	/**WEAPON**/
	primary = sf::Mouse::Left;
	secondary = sf::Mouse::Right;

	cgroup_1 = sf::Keyboard::Num1;
	cgroup_2 = sf::Keyboard::Num2;
	cgroup_3 = sf::Keyboard::Num3;
	cgroup_4 = sf::Keyboard::Num4;

	/**OTHER**/
	store = sf::Keyboard::B;
	respawn = sf::Keyboard::R;
	grabTarget = sf::Keyboard::T;

	buildHardpoint = sf::Keyboard::F;
	buildExtractor = sf::Keyboard::G;
	buildRelay = sf::Keyboard::H;
	buildMind = sf::Keyboard::J;
	buildReplicator = sf::Keyboard::K;

	cameraUp = sf::Keyboard::Up;
	cameraDown = sf::Keyboard::Down;
	cameraLeft = sf::Keyboard::Left;
	cameraRight = sf::Keyboard::Right;
}

Player::Player(const PlayerData& rData) : m_io(rData.ioComp, &Player::input, this), BasePlayerTraits(rData.name), m_resourceUpdateTimer(0.f)
{
	m_hasFocus = true;
	m_inGuiMode = true;
	m_tracking = rData.tracking;
	for(int i = 1; i <= 9; ++i)
	{
		m_weaponGroups[i] = true;
	}
	m_nextTarget = 0;
	m_maxTargets = 1;

	m_resourceUpdateTimer.setCountDown(0.5);
}
Player::~Player()
{

}
Chunk* Player::getChunk() const
{
	Controller* cont = getController();
	Chunk* ship = nullptr;
	if(cont != nullptr)
	{
		ship = cont->getChunk();
	}

	return ship;
}
Controller* Player::getController() const
{
	Controller* cont = nullptr;
	if(m_controller != -1)
	{
		cont = getGame()->getUniverse().getControllerFactory().getController(m_controller);
	}
	return cont;
}
Camera& Player::getCamera()
{
	return m_camera;
}
Vec2 Player::getMouseInWorld()
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
	sf::Mouse::setPosition(sf::Vector2i((int)rPos.x, (int)rPos.y), getGame()->getWindow());
	m_mouseWindowPos = rPos;
}
/// <summary>
/// Indicate which controller our inputs should modify
/// </summary>
void Player::setController(int index)
{
	m_controller = index;
}
/// <summary>
/// Get's the state of keyboard and mouse at this exact moment.
/// </summary>
void Player::getLiveInput()
{
	Controller* cont = nullptr;
	if(m_controller == -1)
		return;

	cont = getGame()->getUniverse().getControllerFactory().getController(m_controller);
	if(cont == nullptr)
		return;

	Controller& rController = *cont;

	rController.toggleLocal(true);
	//default to false
	for(auto it = m_directives.begin(); it != m_directives.end(); ++it)
		it->second = false;

	if(!m_inGuiMode && hasFocus())
	{
		/**== CAMERA ==**/
		const float speed = 0.05f;
		if(sf::Keyboard::isKeyPressed(m_inCfg.cameraUp))
			m_camera.move(Vec2(0, speed));
		if(sf::Keyboard::isKeyPressed(m_inCfg.cameraDown))
			m_camera.move(Vec2(0, -speed));
		if(sf::Keyboard::isKeyPressed(m_inCfg.cameraLeft))
			m_camera.move(Vec2(-speed, 0));
		if(sf::Keyboard::isKeyPressed(m_inCfg.cameraRight))
			m_camera.move(Vec2(speed, 0));


		/**== KEYBOARD ==**/
		if(sf::Keyboard::isKeyPressed(m_inCfg.up))
			m_directives[Directive::Up] = true;
		if(sf::Keyboard::isKeyPressed(m_inCfg.down))
			m_directives[Directive::Down] = true;
		if(sf::Keyboard::isKeyPressed(m_inCfg.rollCCW))
			m_directives[Directive::RollCCW] = true;
		if(sf::Keyboard::isKeyPressed(m_inCfg.rollCW))
			m_directives[Directive::RollCW] = true;
		if(sf::Keyboard::isKeyPressed(m_inCfg.boost))
			m_directives[Directive::Boost] = true;
		if(sf::Keyboard::isKeyPressed(m_inCfg.stealth))
			m_directives[Directive::Stealth] = true;
		if(sf::Keyboard::isKeyPressed(m_inCfg.teleport))
			m_directives[Directive::Teleport] = true;

		if(sf::Keyboard::isKeyPressed(m_inCfg.shield))
			m_directives[Directive::Shield] = true;

		/**== SPECIAL ==**/
		if(sf::Keyboard::isKeyPressed(m_inCfg.store))
		{
			m_directives[Directive::ShowStore] = true;
			std::list<Team> myTeam;
			myTeam.push_back(this->getTeam());
			auto ship = getGame()->getUniverse().getNearestChunk(m_aim, nullptr, myTeam);

			ShipBuilder::shipToGui(ship);
		}
		if(sf::Keyboard::isKeyPressed(m_inCfg.respawn))
			m_directives[Directive::Respawn] = true;


		/**== MOUSE ==**/
		if(sf::Mouse::isButtonPressed(m_inCfg.primary))
			m_directives[Directive::FirePrimary] = true;
		if(sf::Mouse::isButtonPressed(m_inCfg.secondary))
			m_directives[Directive::FireSecondary] = true;

		auto mouseSfmlWorldPos = getGame()->getWindow().mapPixelToCoords(sf::Mouse::getPosition(getGame()->getWindow()), m_camera.getView());
		m_aim = (Vec2)Convert::screenToUniverse(Convert::flipYAxis(mouseSfmlWorldPos));

		/**== DEVELOPER ==**/
		static Timer spawnTimer;
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8))
		{
			if(spawnTimer.isTimeUp())
			{
				spawnTimer.setCountDown(0.1f);
				spawnTimer.restartCountDown();

				getGame()->getUniverse().spawnParticles("LowSparks", m_aim, Vec2(0, 0), Vec2(0, 0));
				getGame()->getSound().playSound("default.wav", m_aim);
			}
		}
	}

	m_mouseWindowPos = getGame()->getWindow().mapPixelToCoords(sf::Mouse::getPosition(getGame()->getWindow()), getGame()->getStaticView());

	CommandInfo commands;
	commands.directives = m_directives;
	commands.weaponGroups = m_weaponGroups;
	commands.isLocal = true;
	rController.locallyUpdate(commands);
	rController.setAim(m_aim);
}
void Player::selectTarget(const Vec2& targetNearPos, const Chunk* playersShip)
{
	Chunk* newTarget = getGame()->getUniverse().getNearestChunk(targetNearPos, playersShip);
	if(newTarget != nullptr)
	{
		if(!hasTarget(newTarget->getFactoryPosition()))//if we don't already have that target
		{
			if(auto oldTarget = m_targets[m_nextTarget])
			{
				getGame()->getUniverse().getChunk(oldTarget)->resetStatusBoard(wptr<leon::Grid>());//reset the old target
			}

			m_targets[m_nextTarget] = newTarget->getFactoryPosition();
			wptr<leon::Grid> grid = m_targetBoards[m_nextTarget];
			newTarget->resetStatusBoard(grid);
		}
		++m_nextTarget;
		m_nextTarget = m_nextTarget % m_maxTargets;
	}
	else
		WARNING;
}
bool Player::hasTarget(int newTarget)
{
	for(auto it = m_targets.cbegin(); it != m_targets.cend(); ++it)
		if(*it == newTarget)
			return true;

	return false;
}
/// <summary>
/// Handle window events such as clicks and gui events.
/// </summary>
void Player::getWindowEvents(sf::RenderWindow& rWindow)//process window events
{
	sf::Event event;


	Controller* cont = nullptr;
	if(m_controller != -1)
		cont = getGame()->getUniverse().getControllerFactory().getController(m_controller);


	while(rWindow.pollEvent(event))
	{
		if(event.type == sf::Event::LostFocus)
			toggleFocus(false);
		if(event.type == sf::Event::GainedFocus)
			toggleFocus(true);
		if(event.type == sf::Event::Resized)
		{
			//TODO WE MAY NEED TO ADJUST OTHER VIEW ELEMENTS???
			getGame()->resizeStaticView();
			m_camera.resize();
			getGame()->getOverlay().getGui().handleEvent(event, false);
		}

		/** CLOSE **/
		if(event.type == sf::Event::Closed)
			rWindow.close();
		if(event.type == sf::Event::KeyPressed)
		{
			if(event.key.code == m_inCfg.buildExtractor)
			{
				build("Extractor", m_aim);
			}
			if(event.key.code == m_inCfg.buildRelay)
			{
				build("Relay", m_aim);
			}
			if(event.key.code == m_inCfg.buildHardpoint)
			{
				build("Hardpoint", m_aim);
			}
			if(event.key.code == m_inCfg.buildMind)
			{
				build("Mind", m_aim);
			}
			if(event.key.code == m_inCfg.buildReplicator)
			{
				build("Replicator", m_aim);
			}
			if(event.key.code == sf::Keyboard::LBracket)
			{
				sf::Packet data;
				data << 1;
				Message::SendUniverse(Message("universe", "upgrade", data, 0.f, false));
			}


			/**== MAIN MENU ==**/
			if(event.key.code == sf::Keyboard::Escape)
			{
				Message menu("overlay", "toggleMenu", voidPacket, 0, false);
				getGame()->getCoreIO().recieve(menu);
			}
			if(event.key.code == m_inCfg.grabTarget)
			{
				const Chunk* myShip;
				if(cont != nullptr)
				{
					myShip = cont->getChunk();
					this->selectTarget(m_aim, myShip);
				}

			}
			/**== SCOREBOARD ==**/
			if(event.key.code == sf::Keyboard::Tab)
			{
				Message scoreboard("overlay", "toggleScoreboard", voidPacket, 0, false);
				getGame()->getCoreIO().recieve(scoreboard);
				//create message to display scoreboard overlay
				//receive message
			}
		}


		/**== GUI ==**/
		if(m_inGuiMode)
		{
			getGame()->getOverlay().handleEvent(event);
		}
		else
		{
			if(cont != nullptr)
			{
				Controller& rController = *cont;

				/**== ZOOM ==**/
				if(event.type == sf::Event::MouseWheelMoved)
				{
					int change = -event.mouseWheel.delta;
					if(change < 0)
						m_camera.setZoom(m_camera.getZoom()*0.8f);
					else
						m_camera.setZoom(m_camera.getZoom()*1.2f);
				}
				auto chunk = rController.getChunk();//make sure we arent over zoomed!
				if(chunk != nullptr)
				{
					float zoomValue = rController.get(Request::Zoom);
					float x1 = (float)getGame()->getWindow().getSize().x; //getting the siz of x
					float y1 = (float)getGame()->getWindow().getSize().y; //getting the size of y
					float area = 1920 * 1080; // setting a fixed resolution
					float area1 = x1*y1;

					float newzoomvalue = zoomValue*sqrt(area / area1); // setting a new zoom value based on the current screen's resolution

					if(newzoomvalue < m_camera.getZoom())
						m_camera.setZoom(newzoomvalue);
				}
			}



			if(event.type == sf::Event::KeyPressed)
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
				if(event.key.code == sf::Keyboard::Numpad9)
					getGame()->getUniverse().toggleDebugDraw();
				if(event.key.code == sf::Keyboard::Numpad5)
					m_tracking = !m_tracking;
				if(event.key.code == sf::Keyboard::Numpad4)
					Print << "\n" << m_aim;
				if(event.key.code == sf::Keyboard::Numpad0)
					getGame()->getUniverse().togglePause();
			}
		}
	}
}
void Player::build(String bpName, Vec2 pos)
{
	if(getGame()->getUniverse().canBuildAtLocation(bpName, pos))
	{
		ChunkDataMessage data;
		data.aiControlled = false;
		data.blueprintName = bpName;
		data.coordinates = m_aim;
		data.needsController = false;
		data.rotation = 0.f;
		data.team = static_cast<int>(getTeam());
		ShipBuilder::Networked::createChunkFromClient(data, 0);
	}
}
void Player::updateView()
{
	//update resources
	if(m_resourceUpdateTimer.isTimeUp())
	{
		m_resourceUpdateTimer.restartCountDown();
		auto resources = getResources();
		if(resources != nullptr) // if we have resources
		{
			auto& myResources = *resources;
			int resourceCounter = 0;
			for(auto it = myResources.m_resourceValues.begin(); it != myResources.m_resourceValues.end(); ++it)
			{
				int32_t resourceValue = it->second;
				sf::Packet data;
				data << resourceValue;
				Message setResource("hud_resource_" + String(resourceCounter), "setValue", data, 0, false);
				getGame()->getCoreIO().recieve(setResource);
				++resourceCounter;
			}
		}
		else
			WARNING;
	}

	//update HUD, camera
	auto chunk = getChunk();
	auto cont = getController();
	if(cont != nullptr && chunk != nullptr)
	{
		Controller& rController = *cont;

		if(chunk->getStatusBoard().expired())
			chunk->resetStatusBoard(m_myStatusBoard);

		if(!m_inGuiMode)
		{
			Vec2 location = chunk->getBodyComponent().getPosition();

			if(m_tracking)
				m_camera.setPosition(location);

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

			//Shield State
			bool isShieldOn = static_cast<bool>(rController.get(Request::ShieldState));
			if(isShieldOn)
			{
				m_shieldState->getAnimator().setAnimation("On", 0.25f);
			}
			else
				m_shieldState->getAnimator().setAnimation("Off", 0.25f);


			//Out of Bounds
			Vec2 bounds = getGame()->getUniverse().getBounds();
			if(abs(location.x) > bounds.x || abs(location.y) > bounds.y)//if out of bounds
				m_boundsDanger->getAnimator().setAnimation("Default", 2.f);


			//m_targetReticules
			int i = 0;
			for(auto it = m_targets.begin(); it != m_targets.cend(); ++it, ++i)
			{
				int chunkPosition = *it;
				Chunk* target = getGame()->getUniverse().getChunk(*it);

				if(target != nullptr)
				{
					m_targetReticules[i]->setPosition(target->getBodyComponent().getPosition());
				}
				else
				{
					*it = FactoryObject::NullPosition;
					m_targetReticules[i]->setPosition(Vec2(-10000, -10000));
				}
			}

			//Radar TODO THIS ALL SHOULD BE IN THE MINIMAP CLASS
			//int index = 0;
			//float maxRange = 50.f;
			//float mapScale = -0.005f;
			//const static Vec2 miniMapCenter = m_minimap->getPosition();
			//m_radarsize = goList.size();
			//for(auto it = goList.begin(); it != goList.end(); ++it)
			//{
			//	//Determine team.
			//	Chunk* object = dynamic_cast<Chunk*>(p);
			//	Team other_team = object->getBodyComponent().getTeam();

			//	sf::Color dotColor(sf::Color::Blue);

			//	if(other_team == Team::Neutral)
			//		dotColor = sf::Color::Blue;
			//	else if(other_team == Team::Capturable)
			//		dotColor = sf::Color(255, 140, 0);
			//	else if(other_team == Team::Invalid)
			//		dotColor = sf::Color::Magenta;
			//	else if(other_team == this->getTeam())
			//		dotColor = sf::Color::Green;
			//	else
			//		dotColor = sf::Color::Red;

			//	if(object != nullptr && !object->isStealth())
			//	{
			//		Vec2 dif = pBody->GetPosition() - object->getBodyPtr()->GetPosition();
			//		float dist = dif.len();

			//		if(dist < maxRange)// TODO fix magic number horror
			//		{

			//			dif *= mapScale;
			//			m_minimap->setDot(miniMapCenter + dif, index, dotColor);
			//			index++;
			//		}
			//	}
			//}
			//m_minimap->cleanMap(index);
		}
	}
}
IOComponent& Player::getIOComp()
{
	return m_io;
}
QuadComponentData createUI(sf::Vector2f size, String displayName)
{
	QuadComponentData data;
	data.dimensions = size;
	data.texName = displayName;
	data.layer = GraphicsLayer::OverlayBottom;

	data.setCenterTopLeft();

	return data;
}
void Player::loadOverlay(const String& rOverlay)
{
	Vec2 winDim((sf::Vector2f)getGame()->getWindow().getSize());
	const float screenDist = 16.f;
	const sf::Vector2f topLeftStart = sf::Vector2f(screenDist, screenDist);//Start of where the top left of the UI should be.

	//Mini Map
	{
		MinimapData data;
		float dims = 256.f;
		data.texName = "overlay/white";
		data.color = sf::Color(96, 96, 96, 32);
		data.dimensions = sf::Vector2f(dims, dims);
		data.setCenterTopLeft();
		data.layer = GraphicsLayer::OverlayMiddle;
		m_minimap.reset(new Minimap(data));
		m_minimap->setGuiPosition(sf::Vector2f(winDim.x - dims - screenDist, winDim.y - dims - screenDist));
	}
	//Energy Bar
	{
		sf::Vector2f energyPos = topLeftStart;

		QuadComponentData data = createUI(sf::Vector2f(32.f, 128.f), "overlay/meter");
		m_energyMeter.reset(new QuadComponent(data));
		m_energyMeter->setGuiPosition(energyPos);

		//Thing covering fill
		LinearMeterData linearData;
		linearData.dimensions = sf::Vector2f(30.f, 124.f);
		linearData.texName = "fill/white";
		linearData.layer = GraphicsLayer::OverlayMiddle;
		linearData.setCenterTopLeft();
		m_energyMeterFill.reset(new LinearMeter(linearData));
		m_energyMeterFill->setGuiPosition(energyPos);
	}
	//Energy Danger
	{
		QuadComponentData data = createUI(sf::Vector2f(86.f, 74.f), "overlay/warning_energy");
		m_energyDanger.reset(new QuadComponent(data));
		m_energyDanger->setGuiPosition(topLeftStart + sf::Vector2f(0.f, 150.f));
	}
	//Shield Display
	{
		QuadComponentData data = createUI(sf::Vector2f(64.f, 64.f), "overlay/shield");
		m_shieldState.reset(new QuadComponent(data));
		m_shieldState->setGuiPosition(topLeftStart + sf::Vector2f(64.f, 0.f));
	}
	//Out of Bounds Warning
	{
		QuadComponentData dataWarnBounds = createUI(sf::Vector2f(250.f, 73.f), "overlay/warning_bounds");
		m_boundsDanger.reset(new QuadComponent(dataWarnBounds));
		m_boundsDanger->setGuiPosition(sf::Vector2f(512.f, -256.f));
	}
	// Create a group icon for each possible group.
	for(int group = 0; group < 4; ++group)
	{
		QuadComponentData groupData = createUI(sf::Vector2f(100.f, 50.f), "overlay/control_group");
		sptr<QuadComponent> groupIcon(new QuadComponent(groupData));
		groupIcon->setGuiPosition(topLeftStart + sf::Vector2f(256.f + 128.f*group, 0.f));

		m_groupIcon.push_back(groupIcon);
	}
	// Target reticules.
	{
		createReticles();
	}
	// Status board
	{
		leon::GridData health;
		health.gridSize = Vec2(48, 48);
		m_myStatusBoard.reset(new leon::Grid(health));

		m_myStatusBoard->m_background->setGuiPosition(sf::Vector2f(screenDist, 512));
	}
	// Target status boards
	{
		m_targets.resize(m_maxTargets);

		for(int i = 0; i < m_maxTargets; ++i)
		{
			leon::GridData gridData;
			gridData.gridSize = Vec2(48, 48);
			sptr<leon::Grid> grid;
			grid.reset(new leon::Grid(gridData));

			grid->m_background->setGuiPosition(sf::Vector2f(winDim.x - screenDist - gridData.gridSize.x*gridData.sizeOfGrid.x, screenDist));
			m_targetBoards.push_back(grid);
		}
		//populate the target grids
	}
}
void Player::createReticles()
{
	m_targetReticules.resize(m_maxTargets);
	for(int i = 0; i < m_maxTargets; ++i)
	{
		QuadComponentData quadData;
		quadData.texName = "overlay/targetMarker";
		quadData.dimensions = sf::Vector2f(2500, 2500);
		quadData.layer = GraphicsLayer::ShipAppendagesUpper;
		auto pQuad = sptr<QuadComponent>(new QuadComponent(quadData));

		m_targetReticules[i] = pQuad;
	}
}
void Player::onBeforeUniverseDestroyed()
{
	m_energyMeter.reset();
	m_shieldState.reset();
	m_energyMeterFill.reset();
	m_energyDanger.reset();
	m_minimap.reset();
	m_boundsDanger.reset();
	m_targetReticules.clear();
	m_myStatusBoard.reset();
	m_targetBoards.clear();
	m_targets.clear();
	// Clear, because otherwise, when we go 
	// to add 4 more to it, the old null
	// pointers are left behind and cause a crash.
	m_groupIcon.clear();
}
void Player::onUniverseCreated()
{
	m_resourceUpdateTimer.restartCountDown();
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
	if(index < (int)m_groupIcon.size())
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
void Player::input(String rCommand, sf::Packet rData)
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
	else if(rCommand == "cameraShake")
	{
		m_camera.shake(0.5f, 60.f, 0.4f);
	}
}
const Resources* Player::getResources() const
{
	return &(getGame()->getUniverse().getTeamResources(getTeam()));
}
bool Player::canChangeResources(const Resources& cost) const
{
	auto resources = getResources();

	Resources available = *resources; // make copy
	available.add(cost);
	return !available.hasNegatives();
}
