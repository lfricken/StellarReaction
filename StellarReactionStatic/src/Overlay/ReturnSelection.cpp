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
	if(rCommand == "show")
	{
		

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
