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
			buttonData.screenCoords = sf::Vector2f(0, 0);
			buttonData.size = sf::Vector2f(1024, 16);
			buttonData.alpha = 0;

			size = sf::Vector2f(1024, 1024);
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
		void f_init(const TooltipData& data);
		void f_update(const sf::Vector2f& rPos);

		virtual bool inputHook(const String rCommand, sf::Packet rData);

		sptr<Button> m_button;
		sptr<DragComponent> m_drag;

		bool m_fading;
		/// <summary>
		/// How long it takes to fade.
		/// </summary>
		Timer m_fadeTimer;
		/// <summary>
		/// How long it takes to start fading.
		/// </summary>
		Timer m_hideTimer;
	};

}
