#include "Overlay.hpp"

#include "Globals.hpp"
#include "Panel.hpp"
#include "Picture.hpp"
#include "Button.hpp"
#include "Courier.hpp"
#include "EditBox.hpp"
#include "Chatbox.hpp"
#include "NetworkedSelection.hpp"

using namespace std;

Overlay::Overlay(const IOComponentData& rData) : m_gui(game.getWindow()), m_io(rData, &Overlay::input, this)
{
	m_gui.setGlobalFont("TGUI/fonts/DejaVuSans.ttf");
	m_menuShowing = true;
	/**If we call loadMenus now, we try and access this very Overlay object before it has been returned to game**/
}
Overlay::~Overlay()
{
	cout << "\nOverlay Destroying...";
}
void Overlay::addPanel(sptr<leon::Panel> spPanel)
{
	m_panelList.push_back(spPanel);
}
void Overlay::handleEvent(sf::Event& rEvent)
{
	m_gui.handleEvent(rEvent);
}
tgui::Gui& Overlay::getGui()
{
	return m_gui;
}







void Overlay::loadMenus()
{


	/**MAIN MENU**/
	/**=========**/
	sf::Packet voidPacket;

	leon::PanelData mainMenuData;
	mainMenuData.ioComp.name = "main_menu";
	mainMenuData.backgroundTex = "core/screen_1.png";
	mainMenuData.screenCoords = sf::Vector2f(0, 0);
	mainMenuData.size = sf::Vector2f(1920,1080);
	leon::Panel* pMain_menu = new leon::Panel(game.getOverlay().getGui(), mainMenuData);
	/**====TITLE====**/
	leon::PictureData pictureData;
	pictureData.texName = "core/main_menu_logo.png";
	pictureData.screenCoords = sf::Vector2f(20, 20);
	pictureData.size = sf::Vector2f(847,104);
	leon::WidgetBase* picture = new leon::Picture(*pMain_menu->getPanelPtr(), pictureData);
	pMain_menu->add(sptr<leon::WidgetBase>(picture));
	/**====RESUME====**/
	leon::ButtonData resumeButtonData;
	resumeButtonData.size = sf::Vector2f(150,50);
	resumeButtonData.buttonText = "Resume";
	resumeButtonData.screenCoords = sf::Vector2f(20, 300);

	Courier resumeMessage1;
	resumeMessage1.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	resumeMessage1.message.reset("overlay", "toggleMenu", voidPacket, 0, false);

	resumeButtonData.ioComp.courierList.push_back(resumeMessage1);
	leon::WidgetBase* pResume = new leon::Button(*pMain_menu->getPanelPtr(), resumeButtonData);
	pMain_menu->add(sptr<leon::WidgetBase>(pResume));
	/**====HOW TO PLAY====**/
	leon::ButtonData htpButData;
	htpButData.size = sf::Vector2f(275,50);
	htpButData.buttonText = "Multiplayer";
	htpButData.screenCoords = sf::Vector2f(20, 400);
	Courier htpMessage;
	htpMessage.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	htpMessage.message.reset("multiplayer_connect", "toggleHidden", voidPacket, 0, false);
	htpButData.ioComp.courierList.push_back(htpMessage);
	leon::WidgetBase* pHTP = new leon::Button(*pMain_menu->getPanelPtr(), htpButData);
	pMain_menu->add(sptr<leon::WidgetBase>(pHTP));
	/**====EXIT====**/
	leon::ButtonData exitButtonData;
	exitButtonData.size = sf::Vector2f(100,50);
	exitButtonData.buttonText = "Exit";
	exitButtonData.screenCoords = sf::Vector2f(20, 600);
	Courier buttonMessage;
	buttonMessage.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	buttonMessage.message.reset("game", "exit", voidPacket, 0, false);
	exitButtonData.ioComp.courierList.push_back(buttonMessage);
	leon::WidgetBase* pExit = new leon::Button(*pMain_menu->getPanelPtr(), exitButtonData);
	pMain_menu->add(sptr<leon::WidgetBase>(pExit));

	game.getOverlay().addPanel(sptr<leon::Panel>(pMain_menu));
	/**=========**/
	/**MAIN MENU**/


	/**MULTIPLAYER**/
	sf::Vector2f multPanelSize = sf::Vector2f(640,480);
	leon::PanelData multiplayerConnect;
	multiplayerConnect.ioComp.name = "multiplayer_connect";
	multiplayerConnect.startHidden = true;
	multiplayerConnect.backgroundColor = sf::Color(50, 50, 50, 128);
	multiplayerConnect.screenCoords = sf::Vector2f(game.getWindow().getSize().x/2-multPanelSize.x/2,game.getWindow().getSize().y/2-multPanelSize.y/2);
	multiplayerConnect.size = sf::Vector2f(multPanelSize.x,multPanelSize.y);
	leon::Panel* pMultMenu = new leon::Panel(*pMain_menu->getPanelPtr(), multiplayerConnect);

	/**JOIN**/
	leon::ButtonData joinButt;
	joinButt.ioComp.name = "join_button";
	joinButt.size = sf::Vector2f(100,50);
	joinButt.buttonText = "Join";
	joinButt.screenCoords = sf::Vector2f(5, 5);
	Courier joinMess;
	joinMess.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	joinMess.message.reset("networkboss", "join", voidPacket, 0, false);
	joinButt.ioComp.courierList.push_back(joinMess);
	pMultMenu->add(sptr<leon::WidgetBase>(new leon::Button(*pMultMenu->getPanelPtr(), joinButt)));
	/**HOST**/
	leon::ButtonData hostButt;
	hostButt.size = sf::Vector2f(100,50);
	hostButt.buttonText = "Host";
	hostButt.screenCoords = sf::Vector2f(110,5);
	Courier hostMess;
	hostMess.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	hostMess.message.reset("networkboss", "host", voidPacket, 0, false);
	hostButt.ioComp.courierList.push_back(hostMess);
	pMultMenu->add(sptr<leon::WidgetBase>(new leon::Button(*pMultMenu->getPanelPtr(), hostButt)));
	/**PORT**/
	leon::EditBoxData port;
	port.size = sf::Vector2f(125,50);
	port.startingText = "5050";
	port.screenCoords = sf::Vector2f(215,5);
	Courier portMess;
	portMess.condition.reset(EventType::TextChanged, 0, 'd', true);
	portMess.message.reset("networkboss", "joinPort", voidPacket, 0, true);
	port.ioComp.courierList.push_back(portMess);
	pMultMenu->add(sptr<leon::WidgetBase>(new leon::EditBox(*pMultMenu->getPanelPtr(), port)));
	/**IP**/
	leon::EditBoxData ipAdd;
	ipAdd.ioComp.name = "ipaddress_editbox";
	ipAdd.size = sf::Vector2f(335,50);
	ipAdd.startingText = "IP Address";
	ipAdd.screenCoords = sf::Vector2f(5,60);
	Courier ipAddMess;
	ipAddMess.condition.reset(EventType::TextChanged, 0, 'd', true);
	ipAddMess.message.reset("networkboss", "joinIP", voidPacket, 0, true);
	ipAdd.ioComp.courierList.push_back(ipAddMess);
	pMultMenu->add(sptr<leon::WidgetBase>(new leon::EditBox(*pMultMenu->getPanelPtr(), ipAdd)));
	/**MULTIPLAYER**/




	/**LOBBY**/
	sf::Vector2f lobbyPanelSize = sf::Vector2f(750,500);
	leon::PanelData lobbyPanel;
	lobbyPanel.ioComp.name = "lobby";
	lobbyPanel.startHidden = true;
	lobbyPanel.backgroundColor = sf::Color(50,50,50,128);
	lobbyPanel.screenCoords = sf::Vector2f(game.getWindow().getSize().x/2-lobbyPanelSize.x/2,game.getWindow().getSize().y/2-lobbyPanelSize.y/2);
	lobbyPanel.size = sf::Vector2f(lobbyPanelSize.x,lobbyPanelSize.y);
	leon::Panel* pLobby = new leon::Panel(*pMain_menu->getPanelPtr(), lobbyPanel);

	/**DISCONNECT**/
	leon::ButtonData disconnect;
	disconnect.ioComp.name = "lobby_disconnect";
	disconnect.size = sf::Vector2f(250,50);
	disconnect.buttonText = "Disconnect";
	disconnect.screenCoords = sf::Vector2f(lobbyPanelSize.x-(disconnect.size.x+5), lobbyPanelSize.y-(disconnect.size.y+5));
	Courier disconnectMess1;
	disconnectMess1.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	disconnectMess1.message.reset("lobby", "toggleHidden", voidPacket, 0, false);
	Courier disconnectMess2;
	disconnectMess2.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	disconnectMess2.message.reset("networkboss", "localOnly", voidPacket, 0, false);

	disconnect.ioComp.courierList.push_back(disconnectMess1);
	disconnect.ioComp.courierList.push_back(disconnectMess2);


	pLobby->add(sptr<leon::WidgetBase>(new leon::Button(*pLobby->getPanelPtr(), disconnect)));

	/**Launch**/
	leon::ButtonData launch;
	launch.ioComp.name = "lobby_launch";
	launch.size = sf::Vector2f(150,50);
	launch.buttonText = "Launch";
	launch.screenCoords = sf::Vector2f(5, lobbyPanelSize.y-(launch.size.y+5));
	Courier launchMess1;
	launchMess1.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	launchMess1.message.reset("networkboss", "launch", voidPacket, 0, false);

	///launch game
	///send data to clients

	launch.ioComp.courierList.push_back(launchMess1);


	pLobby->add(sptr<leon::WidgetBase>(new leon::Button(*pLobby->getPanelPtr(), launch)));

	/**CHATBOX**/
	leon::ChatboxData chatbox;
	chatbox.ioComp.name = "lobby_chatbox";
	chatbox.size = sf::Vector2f(400,300);
	chatbox.screenCoords = sf::Vector2f(3,7);

	pLobby->add(sptr<leon::WidgetBase>(new leon::Chatbox(*pLobby->getPanelPtr(), chatbox)));

	/**SHIP SELECTION**/
	leon::NetworkedSelectionData select;
	select.command = "setShip";
	select.size = sf::Vector2f(200, 200);
	select.screenCoords = sf::Vector2f(420, 7);
	select.ioComp.name = "lobby_shipSelect";
	select.items.push_back(pair<string, int>("CombatShip", 1));
	select.items.push_back(pair<string, int>("DefaultChunk", 2));

	Courier setShip;
	setShip.condition.reset(EventType::Selection, 0, 'd', true);
	setShip.message.reset("networkboss", "sendTcpToHost", voidPacket, 0, true);
	select.ioComp.courierList.push_back(setShip);

	pLobby->add(sptr<leon::WidgetBase>(new leon::NetworkedSelection(*pLobby->getPanelPtr(), select)));

	/**TEAM SELECTION**/
	leon::NetworkedSelectionData selectTeam;
	selectTeam.command = "setTeam";
	selectTeam.size = sf::Vector2f(50, 150);
	selectTeam.itemHeight = 20;
	selectTeam.screenCoords = sf::Vector2f(420, 207);
	selectTeam.ioComp.name = "lobby_teamSelect";
	selectTeam.items.push_back(pair<string, int>("1", 1));
	selectTeam.items.push_back(pair<string, int>("2", 2));
	selectTeam.items.push_back(pair<string, int>("3", 3));
	selectTeam.items.push_back(pair<string, int>("4", 4));

	Courier setTeam;
	setTeam.condition.reset(EventType::Selection, 0, 'd', true);
	setTeam.message.reset("networkboss", "sendTcpToHost", voidPacket, 0, true);
	selectTeam.ioComp.courierList.push_back(setTeam);

	pLobby->add(sptr<leon::WidgetBase>(new leon::NetworkedSelection(*pLobby->getPanelPtr(), selectTeam)));
	/**LOBBY**/





	/**MESSAGE**/
	sf::Vector2f closePanelSize = sf::Vector2f(640,480);
	leon::PanelData messagePanel;
	messagePanel.ioComp.name = "message_panel";
	messagePanel.startHidden = true;
	messagePanel.backgroundColor = sf::Color::Blue;
	messagePanel.screenCoords = sf::Vector2f(game.getWindow().getSize().x/2-closePanelSize.x/2,game.getWindow().getSize().y/2-closePanelSize.y/2);
	messagePanel.size = sf::Vector2f(closePanelSize.x,closePanelSize.y);
	leon::Panel* pMessBox = new leon::Panel(game.getOverlay().getGui(), messagePanel);
	/**====OK====**/
	leon::ButtonData closeMessBox;
	closeMessBox.size = sf::Vector2f(50,50);
	closeMessBox.buttonText = "OK";
	closeMessBox.screenCoords = sf::Vector2f(closePanelSize.x/2-50/2, closePanelSize.y-(50+5));
	Courier closeMess;
	closeMess.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	closeMess.message.reset("message_panel", "toggleHidden", voidPacket, 0, false);
	closeMessBox.ioComp.courierList.push_back(closeMess);
	leon::WidgetBase* pClose = new leon::Button(*pMessBox->getPanelPtr(), closeMessBox);
	pMessBox->add(sptr<leon::WidgetBase>(pClose));
	game.getOverlay().addPanel(sptr<leon::Panel>(pMessBox));

}
void Overlay::toggleMenu(bool show)//display menu, assume gui control, send pause game command
{
	m_menuShowing = show;

	sf::Packet guiMode;
	guiMode << (show);
	sf::Packet hideMenu;
	hideMenu << (!show);
	sf::Packet pause;
	pause << (show);

	Message mes1("main_menu", "setHidden", hideMenu, 0, false);
	Message mes2("local_player", "setGuiMode", guiMode, 0, false);
	Message mes3("universe", "setPause", pause, 0, false);

	game.getCoreIO().recieve(mes1);
	game.getCoreIO().recieve(mes2);
	game.getCoreIO().recieve(mes3);
}
void Overlay::input(const std::string rCommand, sf::Packet rData)
{
	if(rCommand == "toggleMenu")
	{
		toggleMenu(!m_menuShowing);
	}
	else if(rCommand == "setMenu")
	{
		bool show;
		rData >> show;
		toggleMenu(show);
	}
	else
	{
		cout << "\n" << FILELINE;
	}
}
