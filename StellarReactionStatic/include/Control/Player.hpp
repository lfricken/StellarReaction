#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "stdafx.hpp"
#include "Controller.hpp"
#include "Camera.hpp"
#include "BasePlayerTraits.hpp"


class QuadComponent;
class LinearMeter;
class Minimap;

/// <summary>
/// Default key configurations.
/// This system would allow loading and saving of key configurations.
/// </summary>
struct InputConfig
{
	InputConfig() :
		/**MOVEMENT**/
		up(sf::Keyboard::W),
		down(sf::Keyboard::S),
		rollCCW(sf::Keyboard::A),
		rollCW(sf::Keyboard::D),

		/**SPECIAL**/
		stealth(sf::Keyboard::Q),
		shield(sf::Keyboard::Space),
		teleport(sf::Keyboard::E),
		boost(sf::Keyboard::LShift),

		/**WEAPON**/
		primary(sf::Mouse::Left),
		secondary(sf::Mouse::Right),

		cgroup_1(sf::Keyboard::Num1),
		cgroup_2(sf::Keyboard::Num2),
		cgroup_3(sf::Keyboard::Num3),
		cgroup_4(sf::Keyboard::Num4),

		/**OTHER**/
		store(sf::Keyboard::B),

		cameraUp(sf::Keyboard::Up),
		cameraDown(sf::Keyboard::Down),
		cameraLeft(sf::Keyboard::Left),
		cameraRight(sf::Keyboard::Right)
	{}

	/**MOVEMENT**/
	sf::Keyboard::Key up;
	sf::Keyboard::Key down;
	sf::Keyboard::Key rollCCW;
	sf::Keyboard::Key rollCW;

	/**SPECIAL**/
	sf::Keyboard::Key stealth;
	sf::Keyboard::Key shield;
	sf::Keyboard::Key teleport;
	sf::Keyboard::Key boost;

	/**WEAPON**/
	sf::Mouse::Button primary;
	sf::Mouse::Button secondary;

	sf::Keyboard::Key cgroup_1;
	sf::Keyboard::Key cgroup_2;
	sf::Keyboard::Key cgroup_3;
	sf::Keyboard::Key cgroup_4;

	/**OTHER**/
	sf::Keyboard::Key store;

	sf::Keyboard::Key cameraUp;
	sf::Keyboard::Key cameraDown;
	sf::Keyboard::Key cameraLeft;
	sf::Keyboard::Key cameraRight;
};
struct PlayerData
{
	PlayerData() :
		ioComp(&game.getCoreIO()),
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
	b2Vec2 getMouseInWorld();

	/**INPUT**/
	void getLiveInput();//get direct feed from keyboard and mouse, just gets their states though (not events) (up, down, position of mouse)
	void getWindowEvents(sf::RenderWindow& rWindow);//process window events, such as mouse down and key down events


	void updateView();
	void loadOverlay(const std::string& rOverlay);//loads the game HUD (NOT A GUI)

	//this tells the player that the universe no longer exists, meaning the sprites
	//for the HUD need to be destroyed.
	void universeDestroyed();
	bool toggleControlGroup(int group, bool on);
	bool toggleControlGroup(int group);

	int radarsize();

protected:
	void input(std::string rCommand, sf::Packet rData);

private:

	b2Vec2 m_aim;//where we are aiming in the world ATM
	sf::Vector2f m_mouseWindowPos;//where is the players mouse on the screen?
	std::map<Directive, bool> m_directives;//up, down, rollCW, roll CCW, ect.
	std::map<int, bool> m_weaponGroups;//group, is active

	float m_desiredZoom;//for smooth zooming
	b2Vec2 m_desiredCameraPos;//for smooth zooming

	///TEMPORARY
	sptr<QuadComponent> m_energyMeter;
	sptr<QuadComponent> m_energyDanger;
	sptr<QuadComponent> m_boundsDanger;
	sptr<LinearMeter> m_energyMeterFill;
	sptr<Minimap> m_minimap;

	Camera m_camera;//players camera
	InputConfig m_inCfg;
	bool m_inGuiMode;//true if we are in GUI mode
	bool m_tracking;
	bool m_hasFocus;

	int m_radarsize;

	IOComponent m_io;

	string genControlString(); //helper function to populate hud with the player's weapon control groups.
};

#endif // PLAYER_HPP
