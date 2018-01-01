#pragma once

#include "WidgetBase.hpp"

namespace leon
{
	/// Data to initialize a Panel.
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
		String backgroundTex;//background texture
	};

	/// A Panel holds all WidgetBase objects.
	class Panel : public WidgetBase
	{
	public:
		Panel(tgui::Gui& gui, const PanelData& data);
		Panel(tgui::Container& container, const PanelData& data);
		virtual ~Panel();

		/// Return pointer to TGUI::Panel.
		tgui::Panel::Ptr getPanelPtr() const;
		/// Add a WidgetBase to this Panel.
		virtual void add(sptr<WidgetBase> sp_widget);
		/// Delete all objects held by this Panel.
		void clear();

		virtual void mouseMoveToPosition(sf::Vector2f pos);
	protected:
		virtual void load(const String& fullFilePath);
		tgui::Panel::Ptr m_pPanel;//the gui panel
		List<sptr<WidgetBase> > m_widgetList;

	private:
		void f_initialize(const PanelData& data);
	};
}
