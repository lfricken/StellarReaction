#pragma once

#include "Panel.hpp"

namespace leon
{
	class Draggable;

	struct DraggableSurfaceData : public PanelData
	{
		DraggableSurfaceData() :
			PanelData()
		{

		}

		sf::Vector2f gridSize;//how many pixels should represent 1 offset, note that this controls how much can fit on screen
	};
	
	/// <summary>
	/// Contains draggable items to represent the ship
	/// </summary>
	class DraggableSurface : public Panel
	{
	public:
		DraggableSurface(tgui::Gui& gui, const DraggableSurfaceData& rData);
		DraggableSurface(tgui::Container& container, const DraggableSurfaceData& rData);
		~DraggableSurface();

	protected:
	private:
		void f_initialize(const DraggableSurfaceData& data);

		sf::Vector2f m_gridSize;
		std::vector<sptr<Draggable> > m_draggables;
	};
}