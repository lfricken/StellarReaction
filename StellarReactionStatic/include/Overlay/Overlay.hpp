#pragma once

#include "stdafx.hpp"
#include "IOComponent.hpp"
#include "LaunchGame.hpp"
#include "NonCopyable.hpp"

namespace leon
{
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

		/// Handle an SFML Window or Keyboard event.
		void handleEvent(sf::Event& rEvent);
		/// Load the menus.
		void loadMenus();
		/// <summary>
		/// Store
		/// </summary>
		void loadStore(leon::Panel* pStore);
		/// Load scoreboard.
		void loadScoreboard(const GameLaunchData& data);
		/// Return TGUI Gui.
		tgui::Gui& getGui();

		/// Turn the Main Menu on or off. Handles input switching, pausing game, visuals switch.
		void toggleMenu(bool show);
		/// Toggle whether the scoreboard should be visible or not.
		void toggleScoreboard(bool show);

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
