#pragma once

#include "stdafx.hpp"
#include "IOComponent.hpp"
#include "LaunchGame.hpp"
#include "NonCopyable.hpp"
#include "JSON.hpp"



namespace leon
{
	class Panel;

	class StoreLoader
	{
	public:

		/// <summary>
		/// Adds a random button to the store. Returns false if no more buttons can be added.
		/// </summary>
		bool addRandomButton(leon::Panel* pStore);

		struct StoreButtonLoader
		{
			String previewTexture;
			Resources cost;
			String moduleBlueprint;
			String buttonName;

			/// <summary>
			/// Load this data from file.
			/// </summary>
			void loadJson(const Json::Value& root);
		};

		/// <summary>
		/// Load this data from file.
		/// </summary>
		void loadJson(const Json::Value& root);

		List<StoreButtonLoader> buttonList; // List of button data.

	private:
		int lastPosition; // Grid position for the new button.
		List<int> lockedButtons; // List of button indexes that have not been added yet.
		sf::Vector2f buttonSize; // Size of each button in pixels.
		float previewWidth; // Icon width.
		unsigned char baseTransparency; // Transparency of the store.
		sf::Vector2f moduleSpawnPos; // Where to put the buttons when the appear in the editor.
	};

	class Panel;

	/// Holds a list of all Panel, which then contain everything with the GUI.
	class Overlay : NonCopyable
	{
	public:
		Overlay(const IOComponentData& rData);
		virtual ~Overlay();

		/// Return a particular Panel based on its name.
		leon::Panel& getPanel(const String& rPanelName);
		/// Add a Panel to the list.
		void addPanel(sptr<leon::Panel> spPanel);

		/// <summary>
		/// 
		/// </summary>
		void mouseMoveToPosition(sf::Vector2f pos);

		/// Handle an SFML Window or Keyboard event.
		void handleEvent(sf::Event& rEvent);
		/// Load the menus.
		void loadMenus();
		/// <summary>
		/// Store
		/// </summary>
		leon::Panel* loadStore();
		leon::Panel* loadMainMenu();
		leon::Panel* loadSellMenu();
		leon::Panel* loadMultiplayerLobby(leon::Panel* pMain_menu);
		leon::Panel* loadConnectionHub(leon::Panel* pMain_menu);
		leon::Panel* loadHud();
		/// Return TGUI Gui.
		tgui::Gui& getGui();

		void resetStore();

		/// Turn the Main Menu on or off. Handles input switching, pausing game, visuals switch.
		void toggleMenu(bool show);
		/// Toggle whether the scoreboard should be visible or not.
		void toggleScoreboard(bool show);

		/// <summary>
		/// Adds a store button.
		/// </summary>
		bool Overlay::addStoreButton();

		sptr<StoreLoader> storeData; // Data for the store.
		sptr<Panel> storePanel;
	protected:
		void input(const String rCommand, sf::Packet rData);

	private:

		bool m_menuShowing;//true if menu is showing
		bool m_scoreboardShowing;//true if the scoreboard is showing

		IOComponent m_io;
		tgui::Gui m_gui;//the TGUI gui that handles most things
		List<sptr<leon::Panel> > m_panelList;//all the Panels that can get displayed, each item can be active or inactive, if its active, it gets displayed.
	};
}
