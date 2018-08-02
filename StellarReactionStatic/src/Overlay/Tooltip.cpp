#include "Tooltip.hpp"

using namespace leon;

Tooltip::Tooltip(tgui::Gui& gui, const TooltipData& data) : Panel(gui, data), m_fadeTimer(*getGame())
{
	f_init(data);
}
Tooltip::Tooltip(tgui::Container& container, const TooltipData& data) : Panel(container, data), m_fadeTimer(*getGame())
{
	f_init(data);
}
void Tooltip::f_init(const TooltipData& data)
{
	//m_hideTimer.setCountDown(1.f);
	yOffset = 20.f;
	m_fadeTimer.setCountDown(0.2f);
	m_offset = sf::Vector2f(0, 0);

	m_button.reset(new Button(*m_pPanel, data.buttonData));
	add(m_button);

	m_drag.reset(new DragComponent(&Tooltip::f_update, this, &getGame()->getDragUpdater()));
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
	sf::Vector2f pos = rPos + m_offset;
	this->setPosition(pos);

	//control visibility
	if(m_fading)
	{
		f_setTextRelativeAlpha(m_fadeTimer.getTimePercentageElapsed());

		if(m_fadeTimer.isTimeUp())
		{
			m_fading = false;
			f_setTextRelativeAlpha(1.f);
		}
	}
}
bool Tooltip::inputHook(const String rCommand, sf::Packet data)
{
	if(rCommand == "setTooltip")
	{
		TooltipTextData tip;
		tip.fromPacket(&data);

		{
			m_fading = true;
			m_drag->toggleDragging(true);
			m_pPanel->show();
			f_setTextRelativeAlpha(0.f);
		}

		m_fadeTimer.restartCountDown();

		{

			m_textColor = tip.textColor;

			float numLines = (float)(1 + std::count(tip.text.begin(), tip.text.end(), '\n'));
			float lineHeight = (float)tip.textPixelHeight;
			float height = numLines * lineHeight;
			float averageLineWidth = tip.text.size() / numLines;
			float buttonWidth = (averageLineWidth * tip.textPixelHeight) / 2.f;
			float panelWidth = m_pPanel->getSize().x;
			sf::Vector2f size(buttonWidth, height);

			m_pPanel->setBackgroundColor(tip.backColor);
			m_button->setSize(size);
			setSize(size);

			if(tip.align == TooltipTextData::Alignment::RightOfMouse)
				m_offset = sf::Vector2f(0, yOffset);
			else if(tip.align == TooltipTextData::Alignment::CenterOfMouse)
				m_offset = sf::Vector2f(-panelWidth / 2.f, yOffset);
			else if(tip.align == TooltipTextData::Alignment::LeftOfMouse)
				m_offset = sf::Vector2f(-panelWidth, yOffset);

			m_button->m_pButton->setText(tip.text);
		}

		return true;
	}
	else if(rCommand == "unsetTooltip")
	{
		{
			m_fading = false;
			m_drag->toggleDragging(false);
			m_pPanel->hide();//do not call this hide because it will call unsetTooltip
			f_setTextRelativeAlpha(0.f);
		}

		return true;
	}
	else
		return false;
}
void Tooltip::f_setTextRelativeAlpha(float alpha)
{
	sf::Color color;

	color = m_textColor;
	color.a = (char)(color.a * alpha);
	m_button->m_pButton->setTextColor(color);

	color = m_backColor;
	color.a = (char)(color.a * alpha);
	m_pPanel->setBackgroundColor(color);
}

