#include "Overlay.hpp"

#include "Globals.hpp"
#include "Panel.hpp"
#include "Picture.hpp"
#include "Button.hpp"
#include "Courier.hpp"
#include "EditBox.hpp"
#include "Chatbox.hpp"
#include "NetworkedSelection.hpp"
#include "Draggable.hpp"
#include "DraggableSurface.hpp"
#include "Chunk.hpp"
#include <string>

using namespace std;
using namespace leon;

Overlay::Overlay(const IOComponentData& rData) : m_gui(game.getWindow()), m_io(rData, &Overlay::input, this)
{
	m_gui.setGlobalFont(contentDir() + "TGUI/fonts/DejaVuSans.ttf");
	m_menuShowing = true;
	m_scoreboardShowing = false;
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
	mainMenuData.size = sf::Vector2f(1920, 1080);
	leon::Panel* pMain_menu = new leon::Panel(game.getOverlay().getGui(), mainMenuData);
	/**====TITLE====**/
	leon::PictureData pictureData;
	pictureData.texName = "core/main_menu_logo.png";
	pictureData.screenCoords = sf::Vector2f(20, 20);
	pictureData.size = sf::Vector2f(847, 104);
	leon::WidgetBase* picture = new leon::Picture(*pMain_menu->getPanelPtr(), pictureData);
	pMain_menu->add(sptr<leon::WidgetBase>(picture));
	/**====RESUME====**/
	leon::ButtonData resumeButtonData;
	resumeButtonData.size = sf::Vector2f(150, 50);
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
	htpButData.size = sf::Vector2f(275, 50);
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
	exitButtonData.size = sf::Vector2f(100, 50);
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
	sf::Vector2f multPanelSize = sf::Vector2f(640, 480);
	leon::PanelData multiplayerConnect;
	multiplayerConnect.ioComp.name = "multiplayer_connect";
	multiplayerConnect.startHidden = true;
	multiplayerConnect.backgroundColor = sf::Color(50, 50, 50, 128);
	multiplayerConnect.screenCoords = sf::Vector2f(game.getWindow().getSize().x / 2 - multPanelSize.x / 2, game.getWindow().getSize().y / 2 - multPanelSize.y / 2);
	multiplayerConnect.size = sf::Vector2f(multPanelSize.x, multPanelSize.y);
	leon::Panel* pMultMenu = new leon::Panel(*pMain_menu->getPanelPtr(), multiplayerConnect);

	/**JOIN**/
	leon::ButtonData joinButt;
	joinButt.ioComp.name = "join_button";
	joinButt.size = sf::Vector2f(100, 50);
	joinButt.buttonText = "Join";
	joinButt.screenCoords = sf::Vector2f(5, 5);
	Courier joinMess;
	joinMess.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	joinMess.message.reset("networkboss", "join", voidPacket, 0, false);
	joinButt.ioComp.courierList.push_back(joinMess);
	pMultMenu->add(sptr<leon::WidgetBase>(new leon::Button(*pMultMenu->getPanelPtr(), joinButt)));
	/**HOST**/
	leon::ButtonData hostButt;
	hostButt.size = sf::Vector2f(100, 50);
	hostButt.buttonText = "Host";
	hostButt.screenCoords = sf::Vector2f(110, 5);
	Courier hostMess;
	hostMess.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	hostMess.message.reset("networkboss", "host", voidPacket, 0, false);
	hostButt.ioComp.courierList.push_back(hostMess);
	pMultMenu->add(sptr<leon::WidgetBase>(new leon::Button(*pMultMenu->getPanelPtr(), hostButt)));
	/**PORT**/
	leon::EditBoxData port;
	port.size = sf::Vector2f(125, 50);
	port.startingText = "5050";
	port.screenCoords = sf::Vector2f(215, 5);
	Courier portMess;
	portMess.condition.reset(EventType::TextChanged, 0, 'd', true);
	portMess.message.reset("networkboss", "joinPort", voidPacket, 0, true);
	port.ioComp.courierList.push_back(portMess);
	pMultMenu->add(sptr<leon::WidgetBase>(new leon::EditBox(*pMultMenu->getPanelPtr(), port)));
	/**IP**/
	leon::EditBoxData ipAdd;
	ipAdd.ioComp.name = "ipaddress_editbox";
	ipAdd.size = sf::Vector2f(335, 50);
	ipAdd.startingText = "IP Address";
	ipAdd.screenCoords = sf::Vector2f(5, 60);
	Courier ipAddMess;
	ipAddMess.condition.reset(EventType::TextChanged, 0, 'd', true);
	ipAddMess.message.reset("networkboss", "joinIP", voidPacket, 0, true);
	ipAdd.ioComp.courierList.push_back(ipAddMess);
	pMultMenu->add(sptr<leon::WidgetBase>(new leon::EditBox(*pMultMenu->getPanelPtr(), ipAdd)));
	/**MULTIPLAYER**/




	/**LOBBY**/
	sf::Vector2f lobbyPanelSize = sf::Vector2f(750, 500);
	leon::PanelData lobbyPanel;
	lobbyPanel.ioComp.name = "lobby";
	lobbyPanel.startHidden = true;
	lobbyPanel.backgroundColor = sf::Color(50, 50, 50, 128);
	lobbyPanel.screenCoords = sf::Vector2f(game.getWindow().getSize().x / 2 - lobbyPanelSize.x / 2, game.getWindow().getSize().y / 2 - lobbyPanelSize.y / 2);
	lobbyPanel.size = sf::Vector2f(lobbyPanelSize.x, lobbyPanelSize.y);
	leon::Panel* pLobby = new leon::Panel(*pMain_menu->getPanelPtr(), lobbyPanel);

	/**DISCONNECT**/
	leon::ButtonData disconnect;
	disconnect.ioComp.name = "lobby_disconnect";
	disconnect.size = sf::Vector2f(250, 50);
	disconnect.buttonText = "Disconnect";
	disconnect.screenCoords = sf::Vector2f(lobbyPanelSize.x - (disconnect.size.x + 5), lobbyPanelSize.y - (disconnect.size.y + 5));
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
	launch.size = sf::Vector2f(150, 50);
	launch.buttonText = "Launch";
	launch.screenCoords = sf::Vector2f(5, lobbyPanelSize.y - (launch.size.y + 5));
	Courier launchMess1;
	launchMess1.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	launchMess1.message.reset("networkboss", "launch", voidPacket, 0, false);
	Courier launchMess2;
	launchMess2.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	launchMess2.message.reset("hud_panel", "toggleHidden", voidPacket, 0, false);

	///launch game
	///send data to clients

	launch.ioComp.courierList.push_back(launchMess1);
	launch.ioComp.courierList.push_back(launchMess2);

	pLobby->add(sptr<leon::WidgetBase>(new leon::Button(*pLobby->getPanelPtr(), launch)));

	/**CHATBOX**/
	leon::ChatboxData chatbox;
	chatbox.ioComp.name = "lobby_chatbox";
	chatbox.size = sf::Vector2f(400, 300);
	chatbox.screenCoords = sf::Vector2f(3, 7);

	pLobby->add(sptr<leon::WidgetBase>(new leon::Chatbox(*pLobby->getPanelPtr(), chatbox)));



	/**SHIP SELECTION**/
	leon::NetworkedSelectionData select;
	select.size = sf::Vector2f(200, 200);
	select.itemSize = sf::Vector2f(200, 40);
	select.screenCoords = sf::Vector2f(420, 7);
	select.backgroundColor = sf::Color(50, 50, 50, 128);
	select.startHidden = false;
	select.ioComp.name = "lobby_shipSelect";

	leon::SelectableItemData data1;
	data1.texName = "menu/red_menu.png";
	leon::LabelData label1;
	data1.labelData.push_back(label1);

	Courier buttonClick;
	buttonClick.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	buttonClick.message.reset("networkboss", "sendTcpToHost", voidPacket, 0, false);
	data1.buttData.ioComp.courierList.push_back(buttonClick);
	data1.labelData.back().textSize = 16;


	select.command = "setShip";

	data1.labelData.back().text = "Anubis";
	data1.id = "Anubis";
	select.items.push_back(data1);

	data1.labelData.back().text = "Caterina";
	data1.id = "Caterina";
	select.items.push_back(data1);

	data1.labelData.back().text = "Caterina";
	data1.id = "Caterina";
	select.items.push_back(data1);

	data1.labelData.back().text = "Dante";
	data1.id = "Dante";
	select.items.push_back(data1);

	pLobby->add(sptr<leon::WidgetBase>(new leon::NetworkedSelection(*pLobby->getPanelPtr(), select)));



	/**TEAM SELECTION**/
	leon::NetworkedSelectionData selectTeam;
	selectTeam.backgroundColor = sf::Color(50, 50, 50, 128);
	selectTeam.size = sf::Vector2f(200, 160);
	selectTeam.itemSize = sf::Vector2f(200, 40);
	selectTeam.screenCoords = sf::Vector2f(420, 207);
	selectTeam.ioComp.name = "lobby_teamSelect";
	selectTeam.startHidden = false;

	leon::SelectableItemData data;

	leon::LabelData label;
	label.textSize = 16;
	data.labelData.push_back(label);

	Courier mes;
	mes.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	mes.message.reset("networkboss", "sendTcpToHost", voidPacket, 0, false);
	data.buttData.ioComp.courierList.push_back(mes);
	selectTeam.command = "setTeam";

	data.labelData.back().text = "Team 1";
	data.id = "1";
	data.texName = "menu/blue_menu.png";
	selectTeam.items.push_back(data);
	data.labelData.back().text = "Team 2";
	data.id = "2";
	data.texName = "menu/green_menu.png";
	selectTeam.items.push_back(data);
	data.labelData.back().text = "Team 3";
	data.id = "3";
	data.texName = "menu/yellow_menu.png";
	selectTeam.items.push_back(data);
	data.labelData.back().text = "Team 4";
	data.id = "4";
	data.texName = "menu/pink_menu.png";
	selectTeam.items.push_back(data);

	pLobby->add(sptr<leon::WidgetBase>(new leon::NetworkedSelection(*pLobby->getPanelPtr(), selectTeam)));
	/**LOBBY**/
	/**LOBBY**/



	/**STORE PANEL**/
	sf::Vector2f storePanelSize = sf::Vector2f(850, 700);
	leon::PanelData storePanelData;
	storePanelData.ioComp.name = "store_default";
	storePanelData.startHidden = true;
	storePanelData.backgroundColor = sf::Color(50, 50, 50, 128);
	storePanelData.screenCoords = sf::Vector2f(game.getWindow().getSize().x / 2 - storePanelSize.x / 2, game.getWindow().getSize().y / 2 - storePanelSize.y / 2);
	storePanelData.size = sf::Vector2f(storePanelSize.x, storePanelSize.y);
	leon::Panel* pStore = new leon::Panel(game.getOverlay().getGui(), storePanelData);
	/**STORE**/
	leon::NetworkedSelectionData store;
	store.size = sf::Vector2f(200, 400);
	store.itemSize = sf::Vector2f(200, 40);
	store.screenCoords = sf::Vector2f(0, 0);
	store.backgroundColor = sf::Color(50, 50, 50, 128);
	store.startHidden = false;
	store.ioComp.name = "";
	leon::SelectableItemData data2;
	data2.texName = "menu/default_menu.png";
	leon::LabelData buyLabel;
	data2.labelData.push_back(buyLabel);

	Courier storeBuy;
	storeBuy.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	storeBuy.message.reset("networkboss", "sendTcpToHost", voidPacket, 0, false);
	data2.buttData.ioComp.courierList.push_back(storeBuy);
	data2.labelData.back().textSize = 16;


	store.command = "buyModule";

	data2.labelData.back().text = "70mm Autoturret";
	data2.id = "Razor_Ballistic1";
	store.items.push_back(data2);

	data2.labelData.back().text = "Razor 2.5GW Pulse";
	data2.id = "Razor_Laser1";
	store.items.push_back(data2);

	data2.labelData.back().text = "10GW Fusion Reactor";
	data2.id = "DefaultReactor";
	store.items.push_back(data2);

	data2.labelData.back().text = "Titanium Alloy Plate";
	data2.id = "DefaultPlating";
	store.items.push_back(data2);

	data2.labelData.back().text = "100Gigajoule Capacitor";
	data2.id = "DefaultCapacitor";
	store.items.push_back(data2);

	data2.labelData.back().text = "Maxus L2 Engine TM";
	data2.id = "DefaultThruster";
	store.items.push_back(data2);

	data2.labelData.back().text = "Omni Sensor Array";
	data2.id = "DefaultRadar";
	store.items.push_back(data2);

	pStore->add(sptr<leon::WidgetBase>(new leon::NetworkedSelection(*pStore->getPanelPtr(), store)));

	/**SHIP EDITOR**/
	DraggableSurfaceData surfaceData;
	surfaceData.ioComp.name = "ship_editor";
	surfaceData.screenCoords = sf::Vector2f(200, 0);
	surfaceData.gridSize = sf::Vector2f(64, 64);
	surfaceData.size = sf::Vector2f(640, 640);
	surfaceData.backgroundColor = sf::Color(32, 32, 32, 128);


	DraggableSurface* pSurf = new leon::DraggableSurface(*pStore->getPanelPtr(), surfaceData);
	//DraggableSurface* pSurf = new leon::DraggableSurface(*pMain_menu->getPanelPtr(), surfaceData);
	pStore->add(sptr<leon::WidgetBase>(pSurf));
	//pMain_menu->add(sptr<leon::WidgetBase>(pSurf));

	

	/**====DRAGGABLES====**/
	//DraggableData dragData;
	//dragData.size = sf::Vector2f(64, 64);
	//dragData.metaData = "Razor_Laser1";

	//dragData.gridPosition = sf::Vector2f(0, 0);
	//pSurf->addDraggable(dragData);
	//dragData.gridPosition = sf::Vector2f(1, 0);
	//pSurf->addDraggable(dragData);
	//dragData.gridPosition = sf::Vector2f(1, 1);
	//pSurf->addDraggable(dragData);

	/**====RECONSTRUCT SHIP====**/
	ButtonData reconstructData;
	reconstructData.screenCoords = sf::Vector2f(20, 480);
	reconstructData.buttonText = "Reconstruct";

	Courier reconstructButton;
	reconstructButton.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	reconstructButton.message.reset("ship_editor", "sendState", voidPacket, 0, false);
	reconstructData.ioComp.courierList.push_back(reconstructButton);

	pStore->add(sptr<leon::WidgetBase>(new Button(*pStore->getPanelPtr(), reconstructData)));


	/**Close Store**/
	leon::ButtonData close;
	close.ioComp.name = "join_button";
	close.screenCoords = sf::Vector2f(storePanelSize.x - 64, 0);
	close.size = sf::Vector2f(64, 64);
	close.buttonText = "X";
	Courier closeMes;
	closeMes.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	closeMes.message.reset("store_default", "toggleHidden", voidPacket, 0, false);
	close.ioComp.courierList.push_back(closeMes);
	Courier guiMode;
	guiMode.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	guiMode.message.reset("local_player", "toggleGuiMode", voidPacket, 0, false);
	close.ioComp.courierList.push_back(guiMode);
	pStore->add(sptr<leon::WidgetBase>(new leon::Button(*pStore->getPanelPtr(), close)));
	
	
	game.getOverlay().addPanel(sptr<leon::Panel>(pStore));
	/**STORE**/

	/**STORE**/

	/**HUD**/
	/**HUD**/

	/**PANEL**/
	sf::Vector2f textPanelSize = sf::Vector2f(200, 100);
	leon::PanelData hudPanelData;
	hudPanelData.ioComp.name = "hud_panel";
	hudPanelData.startHidden = true;
	hudPanelData.backgroundColor = sf::Color(50, 50, 50, 0);
	hudPanelData.screenCoords = sf::Vector2f(200, 20);
	hudPanelData.size = sf::Vector2f(textPanelSize.x, textPanelSize.y);
	leon::Panel* pHudPanel = new leon::Panel(game.getOverlay().getGui(), hudPanelData);

	/**SCORE**/
	leon::ButtonData hudScore;
	hudScore.ioComp.name = "hud_score";
	hudScore.size = sf::Vector2f(200, 50);
	hudScore.buttonText = "";
	hudScore.transparency = 50;
	hudScore.screenCoords = sf::Vector2f(0, 0);
	pLobby->add(sptr<leon::WidgetBase>(new leon::Button(*pHudPanel->getPanelPtr(), hudScore)));
	/**HUD**/
	/**HUD**/


	/**MESSAGE**/
	sf::Vector2f closePanelSize = sf::Vector2f(640, 480);
	leon::PanelData messagePanel;
	messagePanel.ioComp.name = "message_panel";
	messagePanel.startHidden = true;
	messagePanel.backgroundColor = sf::Color::Blue;
	messagePanel.screenCoords = sf::Vector2f(game.getWindow().getSize().x / 2 - closePanelSize.x / 2, game.getWindow().getSize().y / 2 - closePanelSize.y / 2);
	messagePanel.size = sf::Vector2f(closePanelSize.x, closePanelSize.y);
	leon::Panel* pMessBox = new leon::Panel(game.getOverlay().getGui(), messagePanel);
	/**====OK====**/
	leon::ButtonData closeMessBox;
	closeMessBox.size = sf::Vector2f(50, 50);
	closeMessBox.buttonText = "OK";
	closeMessBox.screenCoords = sf::Vector2f(closePanelSize.x / 2 - 50 / 2, closePanelSize.y - (50 + 5));
	Courier closeMess;
	closeMess.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	closeMess.message.reset("message_panel", "toggleHidden", voidPacket, 0, false);
	closeMessBox.ioComp.courierList.push_back(closeMess);
	leon::WidgetBase* pClose = new leon::Button(*pMessBox->getPanelPtr(), closeMessBox);
	pMessBox->add(sptr<leon::WidgetBase>(pClose));
	game.getOverlay().addPanel(sptr<leon::Panel>(pMessBox));
}
void Overlay::loadScoreboard(const GameLaunchData& data)
{
	// main scoreboard panel
	sf::Vector2f scorebordPanelSize = sf::Vector2f(1280, 720);
	leon::PanelData mainMenuData;
	mainMenuData.ioComp.name = "main_scoreboard";
	mainMenuData.startHidden = true;
	mainMenuData.backgroundColor = sf::Color(50, 50, 50, 128);
	mainMenuData.screenCoords = sf::Vector2f(game.getWindow().getSize().x / 2 - scorebordPanelSize.x / 2, game.getWindow().getSize().y / 2 - scorebordPanelSize.y / 2);
	mainMenuData.size = sf::Vector2f(scorebordPanelSize.x, scorebordPanelSize.y);
	leon::Panel* pMain_menu = new leon::Panel(game.getOverlay().getGui(), mainMenuData);

	//section for team 1, the box will be outlined with blue
	leon::NetworkedSelectionData select;
	select.size = sf::Vector2f(1000, 170);
	select.itemSize = sf::Vector2f(1000, 34);
	select.screenCoords = sf::Vector2f(200, 40);
	select.backgroundColor = sf::Color(50, 50, 50, 128);
	select.startHidden = false;
	select.ioComp.name = "lobby_shipSelect";

	leon::SelectableItemData data1;
	data1.texName = "menu/blue_menu.png";
	leon::LabelData label1;
	data1.labelData.push_back(label1);
	
	Courier buttonClick;
	buttonClick.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	buttonClick.message.reset("networkboss", "sendTcpToHost", voidPacket, 0, false);
	data1.buttData.ioComp.courierList.push_back(buttonClick);
	data1.labelData.back().textSize = 16;


	select.command = "setShip";

	//section for team 2, the box will be outlined with green
	leon::NetworkedSelectionData select2;
	select2.size = sf::Vector2f(1000, 170);
	select2.itemSize = sf::Vector2f(1000, 34);
	select2.screenCoords = sf::Vector2f(200, 210);
	select2.backgroundColor = sf::Color(50, 50, 50, 128);
	select2.startHidden = false;
	select2.ioComp.name = "lobby_shipSelect";

	leon::SelectableItemData data2;
	data2.texName = "menu/green_menu.png";
	leon::LabelData label2;
	data2.labelData.push_back(label2);

	Courier buttonClick2;
	buttonClick2.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	buttonClick2.message.reset("networkboss", "sendTcpToHost", voidPacket, 0, false);
	data2.buttData.ioComp.courierList.push_back(buttonClick2);
	data2.labelData.back().textSize = 16;


	select2.command = "setShip";

	//section for team 3, the box will be outlined with yellow
	leon::NetworkedSelectionData select3;
	select3.size = sf::Vector2f(1000, 170);
	select3.itemSize = sf::Vector2f(1000, 34);
	select3.screenCoords = sf::Vector2f(200, 380);
	select3.backgroundColor = sf::Color(50, 50, 50, 128);
	select3.startHidden = false;
	select3.ioComp.name = "lobby_shipSelect";

	leon::SelectableItemData data3;
	data3.texName = "menu/yellow_menu.png";
	leon::LabelData label3;
	data3.labelData.push_back(label3);

	Courier buttonClick3;
	buttonClick3.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	buttonClick3.message.reset("networkboss", "sendTcpToHost", voidPacket, 0, false);
	data3.buttData.ioComp.courierList.push_back(buttonClick3);
	data3.labelData.back().textSize = 16;


	select3.command = "setShip";

	//section for team 4, the box will be outlined with pink
	leon::NetworkedSelectionData select4;
	select4.size = sf::Vector2f(1000, 170);
	select4.itemSize = sf::Vector2f(1000, 34);
	select4.screenCoords = sf::Vector2f(200, 550);
	select4.backgroundColor = sf::Color(50, 50, 50, 128);
	select4.startHidden = false;
	select4.ioComp.name = "lobby_shipSelect";

	leon::SelectableItemData data4;
	data4.texName = "menu/pink_menu.png";
	leon::LabelData label4;
	data4.labelData.push_back(label4);

	Courier buttonClick4;
	buttonClick4.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
	buttonClick4.message.reset("networkboss", "sendTcpToHost", voidPacket, 0, false);
	data4.buttData.ioComp.courierList.push_back(buttonClick4);
	data4.labelData.back().textSize = 16;


	select4.command = "setShip";

	std::vector< sptr<GameObject> > gamePlayers = game.getUniverse().getPlayerShipList();

	for (auto it = data.playerList.begin(); it != data.playerList.end(); ++it)
	{
		int team = it->team;
		switch (team) {
		case 1:
			data1.labelData.back().text = it->playerName + "                                                             " + "0" + "  " + to_string(it->playerMoney);
			data1.id = it->playerName;
			select.items.push_back(data1);
			break;
		case 2:
			data2.labelData.back().text = it->playerName + "                                                             " + "0" + "  " + to_string(it->playerMoney);
			data2.id = it->playerName;
			select2.items.push_back(data2);
			break;
		case 3:
			data3.labelData.back().text = it->playerName + "                                                             " + "0" + "  " + to_string(it->playerMoney);
			data3.id = it->playerName;
			select3.items.push_back(data3);
			break;
		case 4:
			data4.labelData.back().text = it->playerName + "                                                             " + "0" + "  " + to_string(it->playerMoney);
			data4.id = it->playerName;
			select4.items.push_back(data4);
			break;
		}

	}


	pMain_menu->add(sptr<leon::WidgetBase>(new leon::NetworkedSelection(*pMain_menu->getPanelPtr(), select)));
	pMain_menu->add(sptr<leon::WidgetBase>(new leon::NetworkedSelection(*pMain_menu->getPanelPtr(), select2)));
	pMain_menu->add(sptr<leon::WidgetBase>(new leon::NetworkedSelection(*pMain_menu->getPanelPtr(), select3)));
	pMain_menu->add(sptr<leon::WidgetBase>(new leon::NetworkedSelection(*pMain_menu->getPanelPtr(), select4)));

	leon::ButtonData scoreboardLabel;
	scoreboardLabel.ioComp.name = "label";
	scoreboardLabel.size = sf::Vector2f(1000, 40);
	scoreboardLabel.buttonText = "Player Name                                                             Score  Money";
	scoreboardLabel.screenCoords = sf::Vector2f(200, 5);
	pMain_menu->add(sptr<leon::WidgetBase>(new leon::Button(*pMain_menu->getPanelPtr(), scoreboardLabel)));
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
	sf::Packet hideHUD;
	hideHUD << (show);

	Message mes1("main_menu", "setHidden", hideMenu, 0, false);
	Message mes2("local_player", "setGuiMode", guiMode, 0, false);
	Message mes3("universe", "setPause", pause, 0, false);
	Message mes4("hud_panel", "setHidden", hideHUD, 0, false);

	game.getCoreIO().recieve(mes1);
	game.getCoreIO().recieve(mes2);
	game.getCoreIO().recieve(mes3);
	game.getCoreIO().recieve(mes4);
}
void Overlay::toggleScoreboard(bool show)
{
	m_scoreboardShowing = show;

	sf::Packet hideScoreboard;
	hideScoreboard << (!show);

	Message mes("main_scoreboard", "setHidden", hideScoreboard, 0, false);
	game.getCoreIO().recieve(mes);

}
void Overlay::input(const std::string rCommand, sf::Packet rData)
{
	if(rCommand == "toggleMenu")
	{
		toggleMenu(!m_menuShowing);
	}
	else if (rCommand == "toggleScoreboard")
	{
		toggleScoreboard(!m_scoreboardShowing);
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
