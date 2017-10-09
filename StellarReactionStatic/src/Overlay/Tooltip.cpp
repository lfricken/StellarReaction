#include "Tooltip.hpp"

using namespace leon;

Tooltip::Tooltip(tgui::Gui& gui, const TooltipData& data) : Panel(gui, data), /*m_hideTimer(game),*/ m_fadeTimer(game)
{
	f_init(data);
}
Tooltip::Tooltip(tgui::Container& container, const TooltipData& data) : Panel(container, data), /*m_hideTimer(game),*/ m_fadeTimer(game)
{
	f_init(data);
}
void Tooltip::f_init(const TooltipData& data)
{
	//m_hideTimer.setCountDown(1.f);
	m_fadeTimer.setCountDown(1.f);

	m_button.reset(new Button(*m_pPanel, data.buttonData));
	add(m_button);

	m_drag.reset(new DragComponent(&Tooltip::f_update, this, &game.getDragUpdater()));
	m_fading = false;
	m_drag->toggleDragging(true);

	sf::Packet pack;
	pack << ("hi1 \n hi2 \n hi3 \n hi4");
	inputHook("setTooltip", pack);

}
Tooltip::~Tooltip()
{

}
void Tooltip::f_update(const sf::Vector2f& rPos)
{
	//position us
	sf::Vector2f pos = rPos;
	pos.x -= m_pPanel->getSize().x / 2;
	pos.y += 16;
	this->setPosition(pos);

	//control visibility
	if(m_fading)
	{
		f_setTextAlpha(m_fadeTimer.getTimePercentageElapsed());

		if(m_fadeTimer.isTimeUp())
		{
			m_fading = false;
			f_setTextAlpha(1.f);
		}
	}
}
bool Tooltip::inputHook(const String rCommand, sf::Packet rData)
{
	if(rCommand == "setTooltip")
	{
		{
			m_fading = true;
			m_drag->toggleDragging(true);
			m_pPanel->show();
			f_setTextAlpha(0.f);
		}

		m_fadeTimer.restartCountDown();

		{
			String tipText;
			rData >> tipText;

			float numLines = (float)(1 + std::count(tipText.begin(), tipText.end(), '\n'));
			float lineHeight = m_button->m_pButton->getSize().y;
			float height = numLines * lineHeight;

			float yPos = (height / 2) - lineHeight / 2;
			m_button->setPosition(sf::Vector2f(0, yPos));
			m_button->m_pButton->setText(tipText);
		}

		return true;
	}
	else if(rCommand == "unsetTooltip")
	{
		{
			m_fading = false;
			m_drag->toggleDragging(false);
			m_pPanel->hide();//do not call this hide because it will call unsetTooltip
			f_setTextAlpha(0.f);
		}

		return true;
	}
	else
		return false;
}
void Tooltip::f_setTextAlpha(float alpha)
{
	sf::Color color = m_button->m_pButton->getTextColor();
	color.a = (char)(255.f * alpha);
	m_button->m_pButton->setTextColor(color);
}

