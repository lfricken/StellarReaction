#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "stdafx.hpp"
#include "Controller.hpp"
#include "Camera.hpp"
#include "BasePlayerTraits.hpp"


class DecorQuad;
class LinearMeter;

/// <summary>
/// Default key configurations.
/// This system would allow loading and saving of key configurations.
/// </summary>
struct InputConfig
{
	InputConfig() :
		up(sf::Keyboard::W),
		down(sf::Keyboard::S),
		rollCCW(sf::Keyboard::A),
		rollCW(sf::Keyboard::D),

		primary(sf::Mouse::Left),
		secondary(sf::Mouse::Right),

		nextWeapon(sf::Keyboard::E),
		prevWeapon(sf::Keyboard::Q),


		Stealth(sf::Keyboard::Space),
		special_2(sf::Keyboard::R),
		special_3(sf::Keyboard::F),
		special_4(sf::Keyboard::C),
		special_5(sf::Keyboard::X),
		special_6(sf::Keyboard::Z),
		special_7(sf::Keyboard::LShift),


		cameraUp(sf::Keyboard::Up),
		cameraDown(sf::Keyboard::Down),
		cameraLeft(sf::Keyboard::Left),
		cameraRight(sf::Keyboard::Right),

		boost(sf::Keyboard::LShift),
		store(sf::Keyboard::B)
	{}

	sf::Keyboard::Key up;
	sf::Keyboard::Key down;
	sf::Keyboard::Key rollCCW;
	sf::Keyboard::Key rollCW;

	sf::Mouse::Button primary;
	sf::Mouse::Button secondary;

	sf::Keyboard::Key nextWeapon;
	sf::Keyboard::Key prevWeapon;

	sf::Keyboard::Key Stealth;
	sf::Keyboard::Key special_2;
	sf::Keyboard::Key special_3;
	sf::Keyboard::Key special_4;
	sf::Keyboard::Key special_5;
	sf::Keyboard::Key special_6;
	sf::Keyboard::Key special_7;


	sf::Keyboard::Key cameraUp;
	sf::Keyboard::Key cameraDown;
	sf::Keyboard::Key cameraLeft;
	sf::Keyboard::Key cameraRight;

	sf::Keyboard::Key boost;
	sf::Keyboard::Key store;
};
struct PlayerData
{
	PlayerData() :
		ioComp(game.getCoreIO()),
		keyConfig(),
		tracking(true)
	{
		ioComp.name = "local_player";
		name = "Leon";
	}
	std::string name;
	IOComponentData ioComp;
	InputConfig keyConfig;
	bool tracking;
};


/// <summary>
/// Represents the local player on this machine.
/// This class also handles all user input through it's getLiveInput and getWindowEvents functions
/// those commands are then sent to a controller
/// </summary>
class Player : public BasePlayerTraits
{
public:
	Player(const PlayerData& rData);
	virtual ~Player();

	Camera& getCamera();

	const InputConfig& getInCfg() const;//returns keyboard input configuration
	IOComponent& getIOComp();
	bool inGuiMode() const;//does keyboard and mouse input go to GUI, or game
	bool toggleGuiMode(bool isGuiModeOn);//does keyboard and mouse input go to GUI, or game
	bool toggleFocus(bool isWindowFocused);//toggle focus of application window
	bool hasFocus() const;//Does the application window have operating system focus
	bool isTracking() const;//is our camera tracking our target?
	void setController(int index);//which controller does this player control

	/**MOUSE**/
	const sf::Vector2f& getMouseWindowPos() const;//window coordinates
	void setMouseWindowPos(const sf::Vector2f& rPos);//window coordinates

	/**INPUT**/
	void getLiveInput();//get direct feed from keyboard and mouse, just gets their states though (not events) (up, down, position of mouse)
	void getWindowEvents(sf::RenderWindow& rWindow);//process window events, such as mouse down and key down events


	void updateView();
	void loadOverlay(const std::string& rOverlay);//loads the game HUD (NOT A GUI)

	//this tells the player that the universe no longer exists, meaning the sprites
	//for the HUD need to be destroyed.
	void universeDestroyed();

protected:
	void input(std::string rCommand, sf::Packet rData);

private:

	b2Vec2 m_aim;//where we are aiming in the world ATM
	sf::Vector2f m_mouseWindowPos;//where is the players mouse on the screen?
	std::map<Directive, bool> m_directives;//up, down, rollCW, roll CCW, ect.

	float m_desiredZoom;//for smooth zooming
	b2Vec2 m_desiredCameraPos;//for smooth zooming

	///TEMPORARY
	sptr<DecorQuad> m_energyMeter;
	sptr<DecorQuad> m_energyDanger;
	sptr<LinearMeter> m_energyMeterFill;

	Camera m_camera;//players camera
	InputConfig m_inCfg;
	bool m_inGuiMode;//true if we are in GUI mode
	bool m_tracking;
	bool m_hasFocus;

	IOComponent m_io;
};

#endif // PLAYER_HPP
