#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "stdafx.hpp"
#include "Controller.hpp"
#include "Camera.hpp"


class DecorQuad;
class LinearMeter;

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


		special_1(sf::Keyboard::Space),
		special_2(sf::Keyboard::R),
		special_3(sf::Keyboard::F),
		special_4(sf::Keyboard::C),
		special_5(sf::Keyboard::X),
		special_6(sf::Keyboard::Z),
		special_7(sf::Keyboard::LShift),


		cameraUp(sf::Keyboard::Up),
		cameraDown(sf::Keyboard::Down),
		cameraLeft(sf::Keyboard::Left),
		cameraRight(sf::Keyboard::Right)
	{}

	sf::Keyboard::Key up;
	sf::Keyboard::Key down;
	sf::Keyboard::Key rollCCW;
	sf::Keyboard::Key rollCW;

	sf::Mouse::Button primary;
	sf::Mouse::Button secondary;

	sf::Keyboard::Key nextWeapon;
	sf::Keyboard::Key prevWeapon;

	sf::Keyboard::Key special_1;
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
};
struct PlayerData
{
	PlayerData() :
		ioComp(game.getCoreIO()),
		keyConfig(),
		tracking(true)
	{
		ioComp.name = "local_player";
	}

	IOComponentData ioComp;
	InputConfig keyConfig;
	bool tracking;
};


/**This class is exclusively for 1 local player**/
class Player
{
public:
	Player(const PlayerData& rData);
	virtual ~Player();

	Camera& getCamera();
	const InputConfig& getInCfg() const;
	IOComponent& getIOComp();
	bool inGuiMode() const;//is the player in GUI mode?
	bool toggleGuiMode(bool isGuiModeOn);
	bool toggleFocus(bool isWindowFocused);
	bool hasFocus() const;
	bool isTracking() const;
	void setController(int index);

	/**INPUT**/
	void getLiveInput();//get direct feed from keyboard and mouse, just gets their states though (up, down, position)
	void getWindowEvents(sf::RenderWindow& rWindow);//process window events


	void updateView();
	void loadOverlay(const std::string& rOverlay);

	void universeDestroyed();///this is messed up!, this is not how this should be!

protected:
	void input(std::string rCommand, sf::Packet rData);

private:
	int m_controller;//which controller do we have, 0, 1, 2, ect.(points to a controller in the list)
	b2Vec2 m_aim;//where we are aiming in the world ATM
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
