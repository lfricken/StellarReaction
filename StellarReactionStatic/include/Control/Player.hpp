#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "stdafx.hpp"
#include "Controller.hpp"
#include "Camera.hpp"
#include "BasePlayerTraits.hpp"


class QuadComponent;
class LinearMeter;
class Minimap;
namespace leon
{
	class Grid;
}

/// \brief Key configurations.
///
/// This system would allow loading and saving of key configurations.
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
		respawn(sf::Keyboard::R),
		grabTarget(sf::Keyboard::T),
		buildNewShip(sf::Keyboard::V),

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
	sf::Keyboard::Key grabTarget;

	/**WEAPON**/
	sf::Mouse::Button primary;
	sf::Mouse::Button secondary;

	sf::Keyboard::Key cgroup_1;
	sf::Keyboard::Key cgroup_2;
	sf::Keyboard::Key cgroup_3;
	sf::Keyboard::Key cgroup_4;

	/**OTHER**/
	sf::Keyboard::Key store;
	sf::Keyboard::Key respawn;
	sf::Keyboard::Key buildNewShip;

	sf::Keyboard::Key cameraUp;
	sf::Keyboard::Key cameraDown;
	sf::Keyboard::Key cameraLeft;
	sf::Keyboard::Key cameraRight;
};
/// Data necessary for local player.
struct PlayerData
{
	PlayerData() :
		ioComp(&game.getCoreIO()),
		keyConfig(),
		tracking(true)
	{
		ioComp.name = "local_player";
		name = "default_local_player_name";
	}
	String name;
	IOComponentData ioComp;
	InputConfig keyConfig;
	bool tracking;
};


/// \brief Represents the local player on this machine.
///
/// This class also handles all user input through it's getLiveInput and getWindowEvents functions
/// those commands are then sent to a controller
class Player : public BasePlayerTraits
{
public:
	Player(const PlayerData& rData);
	virtual ~Player();
	/// Returns Camera for local player.
	Camera& getCamera();
	/// Returns keyboard input configuration.
	const InputConfig& getInCfg() const;
	/// Return IO component of the Player.
	IOComponent& getIOComp();
	/// Is the player in GUI mode or Game mode?
	/// If in GUI mode, the commands to go the GUI instead of the players ship.
	bool inGuiMode() const;
	/// Toggle GUI mode on or off.
	bool toggleGuiMode(bool isGuiModeOn);
	/// Toggle whether the Game have "Window Focus"? This is an OS related feature.
	bool toggleFocus(bool isWindowFocused);
	/// Does the Game have Window Focus?
	bool hasFocus() const;
	/// Is the players Camera tracking the players ship?
	bool isTracking() const;
	/// Set which controller this player controlls.
	void setController(int index);

	/// Return the position of the players cursor in window coordinates.
	const sf::Vector2f& getMouseWindowPos() const;
	/// Set the position of the players cursor in window coordinates.
	void setMouseWindowPos(const sf::Vector2f& rPos);
	/// Return the position of the cursor in world coordinates.
	Vec2 getMouseInWorld();

	/// Get direct feed from keyboard and mouse. Gets their states only, not events.
	void getLiveInput();
	/// Get events, such as keydown, keyup, mouse-button-down, mouse-button-up etc.
	void getWindowEvents(sf::RenderWindow& rWindow);

	/// Update HUD elements.
	void updateView();
	/// Initially load HUD elements.
	void loadOverlay(const String& rOverlay);

	/// Tells Player that the universe no longer exists.
	/// This means the sprites for the HUD need to be destroyed if they existed in universe.
	void universeDestroyed();
	/// Set the state of a particular control group.
	bool toggleControlGroup(int group, bool on);
	/// Toggle the state of a control group.
	bool toggleControlGroup(int group);
	/// Number of items on radar.
	int radarsize();

protected:
	void input(String rCommand, sf::Packet rData);

private:
	void selectTarget(const Vec2& targetNearPos, const Chunk* playersShip);
	void createReticles();
	bool hasTarget(const Chunk* target);

	///Where we are aiming in world coordinates.
	Vec2 m_aim;
	sf::Vector2f m_mouseWindowPos;//where is the players mouse on the screen?
	Map<Directive, bool> m_directives;//up, down, rollCW, roll CCW, ect.
	Map<int, bool> m_weaponGroups;//group, is active

	float m_desiredZoom;//for smooth zooming
	Vec2 m_desiredCameraPos;//for smooth zooming

	///TEMPORARY
	sptr<QuadComponent> m_energyMeter;
	sptr<QuadComponent> m_energyDanger;
	sptr<QuadComponent> m_shieldState;
	sptr<QuadComponent> m_boundsDanger;

	int m_nextTarget;
	int m_maxTargets;
	List<wptr<Chunk> > m_targets;
	List<sptr<leon::Grid> > m_targetBoards;
	List<sptr<QuadComponent> > m_targetReticules;

	List<sptr<QuadComponent>> m_groupIcon;

	sptr<LinearMeter> m_energyMeterFill;
	sptr<Minimap> m_minimap;
	sptr<leon::Grid> m_myStatusBoard;

	Camera m_camera;//players camera
	InputConfig m_inCfg;
	bool m_inGuiMode;//true if we are in GUI mode
	bool m_tracking;
	bool m_hasFocus;

	int m_radarsize;

	IOComponent m_io;
};

#endif // PLAYER_HPP
