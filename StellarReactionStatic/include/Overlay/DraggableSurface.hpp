#pragma once

#include "Panel.hpp"

namespace leon
{
	class Draggable;
	struct DraggableData;

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

		void setCountedCoordinates(const std::vector<sf::Vector2f>& rCoords);//which coordinates should we return for getElementPositions
		void addDraggable(const DraggableData& rData);

		//std::vector<std::pair<std::string, sf::Vector2f> > getValidPositions() const;
		std::vector<std::pair<std::string, sf::Vector2f> > getElementPositions() const;
		bool hasOneAt(const sf::Vector2f& gridPos) const;


	protected:
		/**events HOOKS**/
		virtual bool inputHook(const std::string rCommand, sf::Packet rData);

	private:
		void f_initialize(const DraggableSurfaceData& data);

		sf::Vector2f m_gridSize;
		std::vector<sf::Vector2f> m_validCoords;//the list of acceptable coordinates
	};
}