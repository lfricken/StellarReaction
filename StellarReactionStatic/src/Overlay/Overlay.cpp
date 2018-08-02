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
#include "Debugging.hpp"
#include "Grid.hpp"
#include "NumericDisplay.hpp"
#include "ReturnSelection.hpp"
#include "Tooltip.hpp"
#include "JSON.hpp"
#include "Player.hpp"

using namespace leon;

void StoreLoader::StoreButtonLoader::loadJson(const Json::Value& root)
{
	GETJSON(previewTexture);
	GETJSON(cost);
	GETJSON(moduleBlueprint);
	GETJSON(buttonName);

	moduleBlueprint += String((int)getGame()->getLocalPlayer().getTeam());
}
bool StoreLoader::addRandomButton(leon::Panel* pStore)
{
	if(lockedButtons.size() == 0)
		return true;

	sf::Vector2i initialGridPos = (sf::Vector2i)moduleSpawnPos;

	auto butPos = sf::Vector2i(0, lastPosition);
	sf::Vector2i gridsize((int)buttonSize.y, (int)buttonSize.y);

	int index = lockedButtons.back();
	lockedButtons.pop_back();
	auto& button = buttonList[index];

	{//cost display
		NumericDisplayData cost;
		cost.gridPosition = sf::Vector2i(1, lastPosition);
		cost.gridSize = gridsize;
		cost.numDigits = 2;
		cost.digitSize = sf::Vector2f(buttonSize.y / 2, buttonSize.y);
		auto display = new NumericDisplay(*pStore->getPanelPtr(), cost);
		display->setNumber(button.cost.m_resourceValues.find(0)->second);
		pStore->add(sptr<leon::WidgetBase>(display));
	}
	{//icon
		PictureData preview;
		preview.texName = button.previewTexture;
		preview.size.x = buttonSize.y;
		preview.size.y = buttonSize.y;
		preview.gridPosition = butPos;
		preview.gridSize = gridsize;
		pStore->add(sptr<leon::WidgetBase>(new Picture(*pStore->getPanelPtr(), preview)));
	}
	{//purchase button
		sf::Packet moduleInfo;
		String title = button.moduleBlueprint;
		moduleInfo << title;
		moduleInfo << initialGridPos.x << initialGridPos.y;
		button.cost.intoPacket(&moduleInfo);


		Courier purchaseMessage;
		purchaseMessage.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
		purchaseMessage.message.reset("ship_editor", "buyModule", moduleInfo, 0, false);

		leon::ButtonData buyButton;
		buyButton.configFile = "TGUI/widgets/PartialTransparent.conf";
		buyButton.ioComp.name = "buy" + String(lastPosition);
		buyButton.gridPosition = butPos;
		buyButton.gridSize = gridsize;
		buyButton.size = (sf::Vector2f)gridsize;
		buyButton.buttonText = button.buttonName;
		buyButton.startHidden = false;
		buyButton.alpha = 100;
		buyButton.tooltip.text = "DEF: 4\nHP:  100";
		buyButton.tooltip.textColor = sf::Color::White;
		buyButton.tooltip.textPixelHeight = 16;
		buyButton.tooltip.align = TooltipTextData::Alignment::RightOfMouse;
		buyButton.ioComp.courierList.push_back(purchaseMessage);

		pStore->add(sptr<leon::WidgetBase>(new leon::Button(*pStore->getPanelPtr(), buyButton)));
	}

	++lastPosition;
	return false;
}
void StoreLoader::loadJson(const Json::Value& root)
{
	GETJSON(buttonSize);
	GETJSON(previewWidth);
	GETJSON(baseTransparency);
	GETJSON(moduleSpawnPos);

	if(!root["buttonList"].isNull())
	{
		const Json::Value storeButtons = root["buttonList"];
		for(auto it = storeButtons.begin(); it != storeButtons.end(); ++it)
		{
			StoreButtonLoader button;
			button.loadJson(*it);
			buttonList.push_back(button);
		}
	}

	for(int i = 0; i < buttonList.size(); ++i)
	{
		lockedButtons.push_back(i);
	}
	std::random_shuffle(lockedButtons.begin(), lockedButtons.end()); // make the buttons unlock in a random order.
}




Overlay::Overlay(const IOComponentData& rData) : m_gui(getGame()->getWindow()), m_io(rData, &Overlay::input, this)
{
	m_gui.setGlobalFont(contentDir() + "TGUI/fonts/DejaVuSans.ttf");
	m_menuShowing = true;
	m_scoreboardShowing = false;
	/**If we call loadMenus now, we try and access this very Overlay object before it has been returned to game**/
}
Overlay::~Overlay()
{

}
void Overlay::addPanel(sptr<leon::Panel> spPanel)
{
	m_panelList.push_back(spPanel);
}

void Overlay::mouseMoveToPosition(sf::Vector2f pos)
{
	for each(auto panel in m_panelList)
	{
		panel->mouseMoveToPosition(pos);
	}
	if(storePanel)
		storePanel->mouseMoveToPosition(pos);
}
void Overlay::handleEvent(sf::Event& rEvent)
{
	m_gui.handleEvent(rEvent, false);
}
tgui::Gui& Overlay::getGui()
{
	return m_gui;
}
void Overlay::resetStore()
{
	storePanel.reset(loadStore());
}
void Overlay::loadMenus()
{
	auto pMain_menu = loadMainMenu();
	getGame()->getOverlay().addPanel(sptr<leon::Panel>(pMain_menu));
	//load other menus
	getGame()->getOverlay().addPanel(sptr<leon::Panel>(loadConnectionHub(pMain_menu)));
	getGame()->getOverlay().addPanel(sptr<leon::Panel>(loadMultiplayerLobby(pMain_menu)));
	getGame()->getOverlay().addPanel(sptr<leon::Panel>(loadHud()));
	getGame()->getOverlay().addPanel(sptr<leon::Panel>(loadSellMenu()));

	//unlock some initially for debugging


	/**MESSAGE**/
	sf::Vector2f closePanelSize = sf::Vector2f(640, 480);
	leon::PanelData messagePanel;
	messagePanel.ioComp.name = "message_panel";
	messagePanel.startHidden = true;
	messagePanel.backgroundColor = sf::Color::Blue;
	messagePanel.screenCoords = sf::Vector2f(getGame()->getWindow().getSize().x / 2 - closePanelSize.x / 2, getGame()->getWindow().getSize().y / 2 - closePanelSize.y / 2);
	messagePanel.size = sf::Vector2f(closePanelSize.x, closePanelSize.y);
	leon::Panel* pMessBox = new leon::Panel(getGame()->getOverlay().getGui(), messagePanel);
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
	getGame()->getOverlay().addPanel(sptr<leon::Panel>(pMessBox));
}
leon::Panel* Overlay::loadSellMenu()
{
	leon::Panel* panel = nullptr;
	sf::Vector2i panelSize(100, 25);

	{
		leon::ReturnSelectionData data;
		data.ioComp.name = "return_selection";
		data.startHidden = true;
		data.backgroundColor = sf::Color(50, 50, 50, 128);
		data.screenCoords = sf::Vector2f(getGame()->getWindow().getSize().x / 2.f - panelSize.x / 2.f, getGame()->getWindow().getSize().y / 2.f - panelSize.y / 2.f);
		data.size = sf::Vector2f((float)panelSize.x, (float)panelSize.y);
		panel = new leon::ReturnSelection(getGame()->getOverlay().getGui(), data);
	}
	{
		leon::ButtonData sell;
		sell.size = sf::Vector2f(100, 25);
		sell.buttonText = "Sell";
		sell.ioComp.name = "sell_button";
		sell.screenCoords = sf::Vector2f(0, 0);

		panel->add(sptr<leon::WidgetBase>(new leon::Button(*panel->getPanelPtr(), sell)));
	}

	return panel;
}
leon::Panel* Overlay::loadMainMenu()
{
	leon::Panel* pMain_menu = nullptr;
	leon::Panel* pButtons = nullptr;
	//main menu panel
	{
		leon::PanelData mainMenuData;
		mainMenuData.ioComp.name = "main_menu";
		mainMenuData.backgroundTex = "core/screen_1";
		mainMenuData.screenCoords = sf::Vector2f(0, 0);
		mainMenuData.size = sf::Vector2f(1920, 1080);
		pMain_menu = new leon::Panel(getGame()->getOverlay().getGui(), mainMenuData);
	}
	//banner
	{
		leon::PictureData pictureData;
		pictureData.texName = "core/main_menu_logo";
		pictureData.screenCoords = sf::Vector2f(20, 20);
		pictureData.size = sf::Vector2f(847, 104);
		leon::WidgetBase* picture = new leon::Picture(*pMain_menu->getPanelPtr(), pictureData);
		pMain_menu->add(sptr<leon::WidgetBase>(picture));
	}

	//buttons panel
	{
		leon::PanelData mainMenuData;
		mainMenuData.ioComp.name = "main_menu_buttons";
		mainMenuData.backgroundColor = sf::Color::Transparent;
		mainMenuData.screenCoords = sf::Vector2f(0, 0);
		mainMenuData.size = sf::Vector2f(1920, 1080);
		mainMenuData.movesWithMouse = true;
		pButtons = new leon::Panel(*pMain_menu->getPanelPtr(), mainMenuData);
		pMain_menu->add(sptr<leon::WidgetBase>(pButtons));
	}

	//resume button
	{
		leon::ButtonData resumeButtonData;
		resumeButtonData.size = sf::Vector2f(150, 50);
		resumeButtonData.buttonText = "Resume";
		resumeButtonData.screenCoords = sf::Vector2f(20, 300);
		resumeButtonData.startHidden = true;
		resumeButtonData.ioComp.name = "resume_button";

		Courier resumeMessage1;
		resumeMessage1.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
		resumeMessage1.message.reset("overlay", "toggleMenu", voidPacket, 0, false);

		resumeButtonData.ioComp.courierList.push_back(resumeMessage1);
		leon::WidgetBase* pResume = new leon::Button(*pButtons->getPanelPtr(), resumeButtonData);
		pButtons->add(sptr<leon::WidgetBase>(pResume));
	}
	//multiplayer
	{
		leon::ButtonData multiplayer;
		multiplayer.size = sf::Vector2f(275, 50);
		multiplayer.buttonText = "Multiplayer";
		multiplayer.ioComp.name = "multiplayer_button";
		multiplayer.screenCoords = sf::Vector2f(20, 400);

		Courier hideMultiplayerButton;
		//show connection screen
		hideMultiplayerButton.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
		hideMultiplayerButton.message.reset("multiplayer_connect_lobby", "toggleHidden", voidPacket, 0, false);
		multiplayer.ioComp.courierList.push_back(hideMultiplayerButton);

		//hide multiplayer button
		hideMultiplayerButton.message.reset("multiplayer_button", "toggleHidden", voidPacket, 0, false);
		multiplayer.ioComp.courierList.push_back(hideMultiplayerButton);

		pButtons->add(sptr<leon::WidgetBase>(new leon::Button(*pButtons->getPanelPtr(), multiplayer)));
	}
	//exit game
	{
		Courier buttonMessage;
		buttonMessage.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
		buttonMessage.message.reset("game", "exit", voidPacket, 0, false);

		leon::ButtonData exitButtonData;
		exitButtonData.size = sf::Vector2f(100, 50);
		exitButtonData.buttonText = "Exit";
		exitButtonData.screenCoords = sf::Vector2f(20, 600);
		exitButtonData.ioComp.courierList.push_back(buttonMessage);

		pButtons->add(sptr<leon::WidgetBase>(new leon::Button(*pButtons->getPanelPtr(), exitButtonData)));
	}

	return pMain_menu;
}
leon::Panel* Overlay::loadConnectionHub(leon::Panel* pMain_menu)
{
	leon::Panel* pMultMenu = nullptr;

	/**MULTIPLAYER**/
	{
		sf::Vector2f multPanelSize = sf::Vector2f(640, 480);
		leon::PanelData multiplayerConnect;
		multiplayerConnect.ioComp.name = "multiplayer_connect_lobby";
		multiplayerConnect.startHidden = true;
		multiplayerConnect.backgroundColor = sf::Color(50, 50, 50, 128);
		multiplayerConnect.screenCoords = sf::Vector2f(getGame()->getWindow().getSize().x / 2 - multPanelSize.x / 2, getGame()->getWindow().getSize().y / 2 - multPanelSize.y / 2);
		multiplayerConnect.size = sf::Vector2f(multPanelSize.x, multPanelSize.y);
		multiplayerConnect.movesWithMouse = true;
		multiplayerConnect.percievedDistance = 30.f;
		pMultMenu = new leon::Panel(*pMain_menu->getPanelPtr(), multiplayerConnect);
	}
	/**JOIN**/
	{
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
	}
	/**HOST**/
	{
		leon::ButtonData hostButt;
		hostButt.size = sf::Vector2f(100, 50);
		hostButt.buttonText = "Host";
		hostButt.screenCoords = sf::Vector2f(110, 5);
		Courier hostMess;
		hostMess.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
		hostMess.message.reset("networkboss", "host", voidPacket, 0, false);
		hostButt.ioComp.courierList.push_back(hostMess);
		pMultMenu->add(sptr<leon::WidgetBase>(new leon::Button(*pMultMenu->getPanelPtr(), hostButt)));
	}
	/**PORT**/
	{
		leon::EditBoxData port;
		port.size = sf::Vector2f(125, 50);
		port.startingText = "5050";
		port.screenCoords = sf::Vector2f(215, 5);
		Courier portMess;
		portMess.condition.reset(EventType::TextChanged, 0, 'd', true);
		portMess.message.reset("networkboss", "joinPort", voidPacket, 0, true);
		port.ioComp.courierList.push_back(portMess);
		pMultMenu->add(sptr<leon::WidgetBase>(new leon::EditBox(*pMultMenu->getPanelPtr(), port)));
	}
	/**IP**/
	{
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
	}
	return pMultMenu;
}
leon::Panel* Overlay::loadMultiplayerLobby(leon::Panel* pMain_menu)
{
	leon::Panel* pLobby = nullptr;

	sf::Vector2f lobbyPanelSize = sf::Vector2f(750, 500);

	//lobby panel
	{
		leon::PanelData lobbyPanel;
		lobbyPanel.ioComp.name = "lobby";
		lobbyPanel.startHidden = true;
		lobbyPanel.backgroundColor = sf::Color(50, 50, 50, 128);
		lobbyPanel.screenCoords = sf::Vector2f(getGame()->getWindow().getSize().x / 2 - lobbyPanelSize.x / 2, getGame()->getWindow().getSize().y / 2 - lobbyPanelSize.y / 2);
		lobbyPanel.size = sf::Vector2f(lobbyPanelSize.x, lobbyPanelSize.y);
		pLobby = new leon::Panel(*pMain_menu->getPanelPtr(), lobbyPanel);
	}
	// disconnect button
	{
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
	}
	//add AI button
	{
		leon::ButtonData addAI;
		addAI.size = sf::Vector2f(150, 50);
		addAI.buttonText = "AI+";
		addAI.screenCoords = sf::Vector2f(5, lobbyPanelSize.y - (addAI.size.y + 5 + 50));
		Courier aiMessage1;
		aiMessage1.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
		aiMessage1.message.reset("networkboss", "addAI", voidPacket, 0, false);

		addAI.ioComp.courierList.push_back(aiMessage1);

		pLobby->add(sptr<leon::WidgetBase>(new leon::Button(*pLobby->getPanelPtr(), addAI)));
	}
	// launch game button
	{
		leon::ButtonData launch;
		launch.ioComp.name = "lobby_launch";
		launch.size = sf::Vector2f(150, 50);
		launch.buttonText = "Launch";
		launch.screenCoords = sf::Vector2f(5, lobbyPanelSize.y - (launch.size.y + 5));

		Courier launchGame;
		launchGame.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
		launchGame.message.reset("networkboss", "launch", voidPacket, 0, false);

		Courier hideMenu;
		hideMenu.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
		hideMenu.message.reset("hud_panel", "toggleHidden", voidPacket, 0, false);

		launch.ioComp.courierList.push_back(launchGame);
		launch.ioComp.courierList.push_back(hideMenu);

		pLobby->add(sptr<leon::WidgetBase>(new leon::Button(*pLobby->getPanelPtr(), launch)));
	}
	//chatbox
	{
		leon::ChatboxData chatbox;
		chatbox.ioComp.name = "lobby_chatbox";
		chatbox.size = sf::Vector2f(400, 300);
		chatbox.screenCoords = sf::Vector2f(3, 7);

		pLobby->add(sptr<leon::WidgetBase>(new leon::Chatbox(*pLobby->getPanelPtr(), chatbox)));
	}
	// select team
	{
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
		mes.message.reset("networkboss", "Protocol::PlayerOption", voidPacket, 0, false);
		data.buttData.ioComp.courierList.push_back(mes);
		selectTeam.command = "setTeam";

		data.labelData.back().text = "Team 1";
		data.id = "1";
		//data.texName = "menu/blue_menu";
		data.texName = "menu/Blue_menu_button";
		selectTeam.items.push_back(data);
		data.labelData.back().text = "Team 2";
		data.id = "2";
		//data.texName = "menu/green_menu";
		data.texName = "menu/Green_menu_button";
		selectTeam.items.push_back(data);
		data.labelData.back().text = "Team 3";
		data.id = "3";
		//data.texName = "menu/yellow_menu";
		data.texName = "menu/Orange_menu_button";
		selectTeam.items.push_back(data);
		data.labelData.back().text = "Team 4";
		data.id = "4";
		//data.texName = "menu/pink_menu";
		data.texName = "menu/Purple_menu_button";
		selectTeam.items.push_back(data);

		pLobby->add(sptr<leon::WidgetBase>(new leon::NetworkedSelection(*pLobby->getPanelPtr(), selectTeam)));
	}
	// select ship
	{
		leon::NetworkedSelectionData select;
		select.size = sf::Vector2f(200, 200);
		select.itemSize = sf::Vector2f(200, 40);
		select.screenCoords = sf::Vector2f(420, 7);
		select.backgroundColor = sf::Color(50, 50, 50, 128);
		select.startHidden = false;
		select.ioComp.name = "lobby_shipSelect";

		leon::SelectableItemData item;
		//data1.texName = "menu/red_menu";
		item.texName = "menu/Red_menu_button";
		leon::LabelData label1;
		item.labelData.push_back(label1);

		// Action when a networked item is selected.
		Courier buttonClick;
		buttonClick.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
		buttonClick.message.reset("networkboss", "Protocol::PlayerOption", voidPacket, 0, false);//selecting ship
		item.buttData.ioComp.courierList.push_back(buttonClick);
		item.labelData.back().textSize = 16;

		select.command = "setShip";

		item.labelData.back().text = "Anubis";
		item.id = "Anubis";
		select.items.push_back(item);

		item.labelData.back().text = "Caterina";
		item.id = "Caterina";
		select.items.push_back(item);

		item.labelData.back().text = "Caterina";
		item.id = "Caterina";
		select.items.push_back(item);

		item.labelData.back().text = "Dante";
		item.id = "Dante";
		select.items.push_back(item);

		pLobby->add(sptr<leon::WidgetBase>(new leon::NetworkedSelection(*pLobby->getPanelPtr(), select)));
	}

	return pLobby;
}
bool Overlay::addStoreButton()
{
	if(storeData)
		return storeData->addRandomButton(storePanel.get());
	else
	{
		WARNING;
		return false;
	}
}
leon::Panel* Overlay::loadStore()
{
	leon::Panel* pStore = nullptr;
	auto size = getGame()->getWindow().getSize();
	sf::Vector2f storePanelSize = sf::Vector2f(size);

	//load store first
	{
		leon::PanelData storePanelData;
		storePanelData.ioComp.name = "store_default";
		storePanelData.startHidden = true;
		storePanelData.backgroundColor = sf::Color(50, 50, 50, 128);
		storePanelData.screenCoords = sf::Vector2f(getGame()->getWindow().getSize().x / 2 - storePanelSize.x / 2, getGame()->getWindow().getSize().y / 2 - storePanelSize.y / 2);
		storePanelData.size = sf::Vector2f(storePanelSize.x, storePanelSize.y);
		pStore = new leon::Panel(getGame()->getOverlay().getGui(), storePanelData);
	}
	//close store button
	{
		leon::ButtonData close;
		close.ioComp.name = "join_button";
		close.screenCoords = sf::Vector2f(storePanelSize.x - 64 - 128, 4);
		close.size = sf::Vector2f(32, 64);
		close.buttonText = "";

		Courier closeMes;
		closeMes.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
		closeMes.message.reset("store_default", "toggleHidden", voidPacket, 0, false);
		close.ioComp.courierList.push_back(closeMes);

		Courier guiMode;
		guiMode.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
		guiMode.message.reset("local_player", "toggleGuiMode", voidPacket, 0, false);
		close.ioComp.courierList.push_back(guiMode);

		Courier reconstruct;
		reconstruct.condition.reset(EventType::LeftMouseClicked, 0, 'd', true);
		reconstruct.message.reset("ship_editor", "buildShipWithConfiguration", voidPacket, 0, false);
		close.ioComp.courierList.push_back(reconstruct);

		pStore->add(sptr<leon::WidgetBase>(new leon::Button(*pStore->getPanelPtr(), close)));
	}
	//purchase buttons
	{
		std::ifstream store(contentDir() + "blueprints/gui/" + "Store.bp", std::ifstream::binary);
		Json::Reader reader;
		Json::Value root;
		bool parsedSuccess = reader.parse(store, root, false);
		if(parsedSuccess)
		{
			storeData.reset(new StoreLoader());
			storeData->loadJson(root);
		}
		else
			WARNING;
	}
	//ship editor
	{
		sf::Vector2i editGridSize(64, 64);
		sf::Vector2f editGridPos(200.f, 0.f);
		float width = (5.f + 1.f) * editGridSize.x;
		float height = 7.f * editGridSize.y;
		//ship editor background
		{
			PictureData editorBackground;
			editorBackground.texName = "overlay/shipEditor";
			editorBackground.screenCoords = editGridPos;
			editorBackground.gridSize = editGridSize;
			editorBackground.size = sf::Vector2f(width, height);
			pStore->add(sptr<leon::WidgetBase>(new leon::Picture(*pStore->getPanelPtr(), editorBackground)));
		}
		//ship editor
		{
			DraggableSurfaceData surfaceData;
			surfaceData.ioComp.name = "ship_editor";
			surfaceData.screenCoords = editGridPos;
			surfaceData.gridSize = editGridSize;
			surfaceData.size = sf::Vector2f(width, height);
			surfaceData.backgroundColor = sf::Color(32, 32, 32, 128);

			pStore->add(sptr<leon::WidgetBase>(new leon::DraggableSurface(*pStore->getPanelPtr(), surfaceData)));
		}
	}

	{
		TooltipData tipData;
		tipData.ioComp.name = "tooltip";
		tipData.screenCoords = sf::Vector2f(512, 512);

		pStore->add(sptr<leon::WidgetBase>(new leon::Tooltip(*pStore->getPanelPtr(), tipData)));
	}

	return pStore;
}
leon::Panel* Overlay::loadHud()
{
	leon::Panel* pHudPanel = nullptr;

	//hud panel
	{
		sf::Vector2f textPanelSize = sf::Vector2f(256, 256);
		leon::PanelData hudPanelData;
		hudPanelData.ioComp.name = "hud_panel";
		hudPanelData.startHidden = true;
		hudPanelData.backgroundColor = sf::Color(50, 50, 50, 0);
		hudPanelData.screenCoords = sf::Vector2f(768, 16);
		hudPanelData.size = sf::Vector2f(textPanelSize.x, textPanelSize.y);
		pHudPanel = new leon::Panel(getGame()->getOverlay().getGui(), hudPanelData);
	}
	//resources
	Resources loop;
	int spacing = 32;
	leon::NumericDisplayData hudNumbers;
	hudNumbers.screenCoords = sf::Vector2f(0, 0);
	hudNumbers.digitSize = sf::Vector2f(20, 40);
	hudNumbers.numDigits = 3;

	int i = 0;
	for(auto it = loop.m_resourceValues.begin(); it != loop.m_resourceValues.end(); ++it)
	{
		hudNumbers.ioComp.name = "hud_resource_" + String(i);

		auto num = new leon::NumericDisplay(*pHudPanel->getPanelPtr(), hudNumbers);
		num->setNumber(i);
		pHudPanel->add(sptr<leon::WidgetBase>(num));


		hudNumbers.screenCoords.x += hudNumbers.numDigits * hudNumbers.digitSize.x + spacing;
		++i;
	}

	return pHudPanel;
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

	Universe* x = &getGame()->getUniverse();

	if(x->started)
	{

		Message mes1("main_menu", "setHidden", hideMenu, 0, false);
		Message mes2("local_player", "setGuiMode", guiMode, 0, false);
		Message mes3("universe", "setPause", pause, 0, false);
		Message mes4("hud_panel", "setHidden", hideHUD, 0, false);
		Message mes5("resume_button", "setHidden", hideMenu, 0, false);

		getGame()->getCoreIO().recieve(mes1);
		getGame()->getCoreIO().recieve(mes2);
		getGame()->getCoreIO().recieve(mes3);
		getGame()->getCoreIO().recieve(mes4);
		getGame()->getCoreIO().recieve(mes5);

	}

}
void Overlay::toggleScoreboard(bool show)
{

}
void Overlay::input(const String rCommand, sf::Packet rData)
{
	if(rCommand == "toggleMenu")
	{
		toggleMenu(!m_menuShowing);
	}
	else if(rCommand == "toggleScoreboard")
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
		WARNING;
	}
}



