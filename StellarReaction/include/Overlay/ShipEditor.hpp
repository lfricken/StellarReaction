#pragma once

#include "Panel.hpp"

namespace leon
{
	struct ShipEditorData : public PanelData
	{
		ShipEditorData() :
			PanelData()
		{

		}

		sf::Vector2f gridSize;//how many pixels should represent 1 offset, note that this controls how much can fit on screen
	};
	
	/// <summary>
	/// Contains draggable items to represent the ship
	/// </summary>
	class ShipEditor : public Panel
	{
	public:
		ShipEditor(tgui::Gui& gui, const ShipEditorData& rData);
		ShipEditor(tgui::Container& container, const ShipEditorData& rData);
		~ShipEditor();

	protected:
	private:
	};
}