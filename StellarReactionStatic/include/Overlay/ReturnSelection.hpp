#pragma once

#include "Panel.hpp"


namespace leon
{
	/// Data to initialize a Panel.
	struct ReturnSelectionData : public PanelData
	{
		ReturnSelectionData() :
			PanelData()
		{
		}
	};

	class ReturnSelection : public Panel
	{
	public:
		ReturnSelection(tgui::Gui& gui, const ReturnSelectionData& data);
		ReturnSelection(tgui::Container& container, const ReturnSelectionData& data);
		virtual ~ReturnSelection();
	protected:

		virtual bool inputHook(const String rCommand, sf::Packet rData);
	};
}
