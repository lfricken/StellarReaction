#pragma once

#include "Panel.hpp"

namespace leon
{
	class Draggable;
	struct DraggableData;

	/// Data to initialize DraggableSurface.
	struct DraggableSurfaceData : public PanelData
	{
		DraggableSurfaceData() :
			PanelData(),
			gridOffset(3, 3)
		{

		}
		/// Size in pixels of one grid slot.
		sf::Vector2i gridOffset;
	};
	
	/// A Panel specialized to for the Draggable.
	class DraggableSurface : public Panel
	{
	public:
		DraggableSurface(tgui::Gui& gui, const DraggableSurfaceData& rData);
		DraggableSurface(tgui::Container& container, const DraggableSurfaceData& rData);
		~DraggableSurface();

		/// Which coordinates should the Draggable objects be considered. Has no effect at the moment.
		void setCountedCoordinates(const List<sf::Vector2i>& rCoords);
		/// Add a draggable object.
		void addDraggable(const DraggableData& rData);

		// Currently commented out for unknown reason.
		//List<std::pair<String, sf::Vector2f> > getValidPositions() const;

		/// Get a list of the Draggable data, as well as their positions.
		List<std::pair<String, sf::Vector2i> > getElementGridPositions() const;
		/// Returns true whether we have a Draggable at the specified grid position. Used by Draggable::trySetPosition
		bool hasOneAt(const sf::Vector2i& gridPos) const;

		Vec2 toWorldCoords(const sf::Vector2i& gridCoord) const;
		sf::Vector2i fromWorldCoords(const Vec2& worldCoord) const;

	protected:
		/**events HOOKS**/
		virtual bool inputHook(const String rCommand, sf::Packet rData);

	private:
		void f_initialize(const DraggableSurfaceData& data);

		/// Size in pixels of one grid slot.
		sf::Vector2i m_gridOffset;
		List<sf::Vector2i> m_validGridCoords;//the list of acceptable coordinates
	};
}