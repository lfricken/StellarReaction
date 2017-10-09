#pragma once

#include "Panel.hpp"
#include "Button.hpp"
#include "DragComponent.hpp"

namespace leon
{
	struct TooltipData : PanelData
	{
		TooltipData()
		{
			float tooltipWidth = 1024;

			buttonData.screenCoords = sf::Vector2f(0, 0);
			buttonData.size = sf::Vector2f(tooltipWidth, 16);//button must be centered
			buttonData.alpha = 0;

			size = sf::Vector2f(tooltipWidth, 1024);
			alpha = 0;
			backgroundColor = sf::Color(255, 255, 255, 0);
		}

		ButtonData buttonData;
	};

	class Tooltip : public Panel
	{
	public:
		Tooltip(tgui::Gui& gui, const TooltipData& data);
		Tooltip(tgui::Container& container, const TooltipData& data);
		virtual ~Tooltip();


	private:

		/// <summary>
		/// Take alpha as 0-1, where 0 is invisible, 1 is max m_color.a
		/// </summary>
		void f_setTextRelativeAlpha(float alpha);
		void f_init(const TooltipData& data);
		void f_update(const sf::Vector2f& rPos);

		virtual bool inputHook(const String rCommand, sf::Packet rData);

		sptr<Button> m_button;
		sptr<DragComponent> m_drag;

		/// <summary>
		/// Have we entered fading transition yet.
		/// </summary>
		bool m_fading;
		/// <summary>
		/// How long it takes to fade.
		/// </summary>
		Timer m_fadeTimer;
		/// <summary>
		/// What color should the text be when fully shown?
		/// </summary>
		sf::Color m_textColor;
	};

}
