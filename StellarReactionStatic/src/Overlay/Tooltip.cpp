#include "Tooltip.hpp"

using namespace leon;

Tooltip::Tooltip(tgui::Gui& gui, const TooltipData& data) : Panel(gui, data), m_hideTimer(game), m_fadeTimer(game)
{
	f_init(data);
}
Tooltip::Tooltip(tgui::Container& container, const TooltipData& data) : Panel(container, data), m_hideTimer(game), m_fadeTimer(game)
{
	f_init(data);
}
void Tooltip::f_init(const TooltipData& data)
{
	m_hideTimer.setCountDown(1.f);
	m_fadeTimer.setCountDown(0.25f);

	m_button.reset(new Button(*m_pPanel, data.buttonData));
	add(m_button);

	m_drag.reset(new DragComponent(&Tooltip::f_update, this, &game.getDragUpdater()));
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
	sf::Vector2f pos = rPos;
	pos.x -= m_pPanel->getSize().x / 2;
	pos.y += 16;
	this->setPosition(pos);

	if(m_hideTimer.isTimeUp())
	{
		m_fadeTimer.restartCountDown();
		m_fading = true;
	}

	if(m_fading)
	{
		//m_button->m_pButton->setTextColor();
		sf::Color color = m_pPanel->getBackgroundColor();
		color.a = (char)(255 - (255.f * m_fadeTimer.getTimePercentageElapsed()));
		m_pPanel->setBackgroundColor(color);

		if(m_fadeTimer.isTimeUp())
		{
			m_drag->toggleDragging(false);
			hide();
		}
	}
}
bool Tooltip::inputHook(const String rCommand, sf::Packet rData)
{
	if(rCommand == "setTooltip")
	{
		show();
		m_drag->toggleDragging(true);

		m_hideTimer.restartCountDown();
		m_fading = false;

		String tip;
		rData >> tip;

		float numLines = (float)(1 + std::count(tip.begin(), tip.end(), '\n'));
		float lineHeight = m_button->m_pButton->getSize().y;
		float height = numLines * lineHeight;

		float yPos = (height / 2) - lineHeight / 2;
		m_button->setPosition(sf::Vector2f(0, yPos));
		m_button->m_pButton->setText(tip);

		return true;
	}
	else
		return false;
}
