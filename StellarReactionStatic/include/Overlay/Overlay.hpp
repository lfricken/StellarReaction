#pragma once

#include "stdafx.hpp"
#include "IOComponent.hpp"
#include "LaunchGame.hpp"
#include "NonCopyable.hpp"

namespace leon
{
	class Panel;

	class Overlay : NonCopyable
	{
	public:
		Overlay(const IOComponentData& rData);
		virtual ~Overlay();

		leon::Panel& getPanel(const std::string& rPanelName);
		void addPanel(sptr<leon::Panel> spPanel);

		void handleEvent(sf::Event& rEvent);
		void loadMenus();
		void loadScoreboard(const GameLaunchData& data);
		tgui::Gui& getGui();

		void toggleMenu(bool show);//display menu, assume gui control, send pause game command
		void toggleScoreboard(bool show);

	protected:
		void input(const std::string rCommand, sf::Packet rData);

	private:
		bool m_menuShowing;//true if menu is showing
		bool m_scoreboardShowing;//true if the scoreboard is showing

		IOComponent m_io;
		tgui::Gui m_gui;//the TGUI gui that handles most things
		std::vector<sptr<leon::Panel> > m_panelList;//all the Panels that can get displayed, each item can be active or inactive, if its active, it gets displayed.
	};
}
