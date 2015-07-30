#include "Player.hpp"
#include "Globals.hpp"
#include "Overlay.hpp"
#include "Panel.hpp"
#include "Universe.hpp"
#include "IOManager.hpp"
#include "Convert.hpp"
#include "DecorQuad.hpp"
#include "LinearMeter.hpp"

using namespace std;
using namespace sf;

Player::Player(const PlayerData& rData) : m_io(rData.ioComp, &Player::input, this)
{
	m_controller = 999999;//by default they have the 0 controller
	m_hasFocus = true;
	m_inGuiMode = true;
	m_tracking = rData.tracking;
}
Player::~Player()
{
	cout << "\nPlayer Destroying...";
}
Camera& Player::getCamera()
{
	return m_camera;
}
const InputConfig& Player::getInCfg() const
{
	return m_inCfg;
}
/// <summary>
/// Are the player inputs going to the gui or the controller
/// </summary>
/// <returns></returns>
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
/// <returns></returns>
bool Player::isTracking() const
{
	return m_tracking;
}
/// <summary>
/// Indicate which controller our inputs should modify
/// </summary>
/// <param name="index">The index.</param>
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
	if(!m_inGuiMode && hasFocus())
	{
		/**== CAMERA ==**/
		const float speed = 0.05f;
		if(Keyboard::isKeyPressed(m_inCfg.cameraUp))
			m_camera.move(b2Vec2(0,speed));
		if(Keyboard::isKeyPressed(m_inCfg.cameraDown))
			m_camera.move(b2Vec2(0,-speed));
		if(Keyboard::isKeyPressed(m_inCfg.cameraLeft))
			m_camera.move(b2Vec2(-speed,0));
		if(Keyboard::isKeyPressed(m_inCfg.cameraRight))
			m_camera.move(b2Vec2(speed,0));


		/**== KEYBOARD ==**/
		if(Keyboard::isKeyPressed(m_inCfg.up))
			m_directives[Directive::Up] = true;
		else
			m_directives[Directive::Up] = false;
		if(Keyboard::isKeyPressed(m_inCfg.down))
			m_directives[Directive::Down] = true;
		else
			m_directives[Directive::Down] = false;
		if(Keyboard::isKeyPressed(m_inCfg.rollCCW))
			m_directives[Directive::RollCCW] = true;
		else
			m_directives[Directive::RollCCW] = false;
		if(Keyboard::isKeyPressed(m_inCfg.rollCW))
			m_directives[Directive::RollCW] = true;
		else
			m_directives[Directive::RollCW] = false;

		/**== MOUSE **/
		if(Mouse::isButtonPressed(m_inCfg.primary))
			m_directives[Directive::FirePrimary] = true;
		else
			m_directives[Directive::FirePrimary] = false;
		if(Mouse::isButtonPressed(m_inCfg.secondary))
			m_directives[Directive::FireSecondary] = true;
		else
			m_directives[Directive::FireSecondary] = false;

		m_aim = leon::sfTob2(game.getWindow().mapPixelToCoords(Mouse::getPosition(game.getWindow()), m_camera.getView()));

		/**== DEVELOPER ==**/
		if(Keyboard::isKeyPressed(Keyboard::G))
			cout << "\n(" << m_aim.x << ",\t" << m_aim.y << ")";

		Controller& rController = game.getUniverse().getControllerFactory().getController(m_controller);
		rController.updateDirectives(m_directives);
		rController.setAim(m_aim);
	}
}
/// <summary>
/// Hande window events such as clicks and gui events.
/// </summary>
/// <param name="rWindow">The r window.</param>
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
					m_camera.setZoom(m_camera.getZoom()*0.8);
				else
					m_camera.setZoom(m_camera.getZoom()*1.2);
			}
			b2Body* pBody = rController.getBodyPtr();//make sure we arent over zoomed!
			if(pBody != NULL)
			{
				float zoomValue = rController.get(Request::Zoom);
				if(zoomValue < m_camera.getZoom())
					m_camera.setZoom(zoomValue);
			}


			/**== DEVELOPER OPTIONS ==**/
			if(event.type == Event::KeyPressed)
			{
				if(event.key.code == Keyboard::O)
					game.getUniverse().toggleDebugDraw();
				if(event.key.code == Keyboard::T)
					m_tracking = !m_tracking;
				if(event.key.code == Keyboard::P)
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

		float val = rController.get(Request::Energy);
		float maxVal = rController.get(Request::MaxEnergy);
		if(maxVal <= 0.f)
		{
			maxVal = 1.f;
			val = 0.f;
		}

		m_energyMeterFill->setPercent(val/maxVal);
		if(val/maxVal < 0.1f)
		{
			string com = "setAnimation";
			sf::Packet dat;
			dat << "Default";
			dat << 2.f;
			m_energyDanger->input(com, dat);
		}


	}
}
IOComponent& Player::getIOComp()
{
	return m_io;
}
void Player::loadOverlay(const std::string& rOverlay)
{
	b2Vec2 emeterPos = b2Vec2(0.2f, -0.45f);

	LinearMeterData fillData;
	fillData.dimensions = sf::Vector2f(30,124);
	fillData.layer = GraphicsLayer::OverlayMiddle;
	LinearMeter* pFill = new LinearMeter(fillData);
	pFill->setPosition(emeterPos);


	DecorQuadData data;
	data.quadComp.dimensions = sf::Vector2f(32,128);
	data.quadComp.texName = "overlay/meter.png";
	data.quadComp.animSheetName = "overlay/meter.acfg";
	data.quadComp.layer = GraphicsLayer::Overlay;
	DecorQuad* pDQuad = new DecorQuad(data);
	pDQuad->setPosition(emeterPos);


	DecorQuadData datawarn;
	datawarn.quadComp.dimensions = sf::Vector2f(86,73);
	datawarn.quadComp.texName = "overlay/warning_energy.png";
	datawarn.quadComp.animSheetName = "overlay/warning_energy.acfg";
	datawarn.quadComp.layer = GraphicsLayer::Overlay;
	DecorQuad* pDang = new DecorQuad(datawarn);
	pDang->setPosition(emeterPos+b2Vec2(0.f, -0.4f));




	m_energyMeter = sptr<DecorQuad>(pDQuad);
	m_energyMeterFill = sptr<LinearMeter>(pFill);
	m_energyDanger = sptr<DecorQuad>(pDang);


}
void Player::universeDestroyed()
{
	m_energyMeter.reset();
	m_energyMeterFill.reset();
	m_energyDanger.reset();
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
