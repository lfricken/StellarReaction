#include "ReturnSelection.hpp"

using namespace leon;

ReturnSelection::ReturnSelection(tgui::Gui& gui, const ReturnSelectionData& rData) : Panel(gui, rData)
{

}
ReturnSelection::ReturnSelection(tgui::Container& container, const ReturnSelectionData& rData) : Panel(container, rData)
{

}
ReturnSelection::~ReturnSelection()
{

}
bool ReturnSelection::inputHook(const String rCommand, sf::Packet rData)
{
	if(rCommand == "show_sellscreen")
	{
		sf::Vector2i pos;
		sf::Vector2i widgetGridPos;
		bool center;
		bool mouseOffClose;

		rData >> pos.x;
		rData >> pos.y;
		rData >> widgetGridPos.x;
		rData >> widgetGridPos.y;
		rData >> center;
		rData >> mouseOffClose;


		sf::Vector2i size = (sf::Vector2i)m_pPanel->getSize();
		//setPosition(sf::Vector2f((float)pos.x, (float)pos.y));
		if(center)
		{
			setPosition(sf::Vector2f((float)pos.x - (size.x / 2), (float)pos.y - (size.y / 2)));
		}
		else
		{
			setPosition(sf::Vector2f((float)pos.x, (float)pos.y));
		}

		m_io.getEventer().clear();
		if(mouseOffClose)
		{
			{
				m_io.getEventer().clear();

				Courier c;
				c.condition = Condition(EventType::MouseLeft, 0, 'd', false);
				c.message = Message(m_io.getName(), "hide", voidPacket, 0.f, false);
				m_io.getEventer().add(c);
			}
			{
				m_widgetList[0]->m_io.getEventer().clear();

				sf::Packet targetModulePos;
				targetModulePos << widgetGridPos.x;
				targetModulePos << widgetGridPos.y;

				Courier c;
				c.condition = Condition(EventType::LeftMouseClicked, 0, 'd', false);
				c.message = Message("ship_editor", "sell", targetModulePos, 0.f, false);
				m_widgetList[0]->m_io.getEventer().add(c);
			}
		}

		toggleHidden(false);
		return true;
	}
	if(rCommand == "hide")
	{


		toggleHidden(true);
		return true;
	}
	else
	{
		return false;
	}
}
