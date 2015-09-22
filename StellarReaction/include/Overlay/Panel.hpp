#pragma once

#include "WidgetBase.hpp"

namespace leon
{

	struct PanelData : public WidgetBaseData
	{
		PanelData() :
			WidgetBaseData(),
			///hidePanelButton(false),
			backgroundColor(sf::Color(255,0,255,128))
		{
		}

		///bool hidePanelButton;//should we hide the button that closes this panel
		sf::Color backgroundColor;//background color if not given a texture
		std::string backgroundTex;//background texture
	};

	class Panel : public WidgetBase
	{
	public:
		Panel(tgui::Gui& gui, const PanelData& data);
		Panel(tgui::Container& container, const PanelData& data);
		~Panel();

		tgui::Panel::Ptr getPanelPtr() const;
		void add(sptr<WidgetBase> sp_widget);
		void clear();

	protected:
		tgui::Panel::Ptr m_pPanel;//the gui panel
		std::vector<sptr<WidgetBase> > m_widgetList;

	private:
		void f_initialize(const PanelData& data);

	};
}
