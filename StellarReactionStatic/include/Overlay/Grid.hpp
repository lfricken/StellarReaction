#pragma once
#include "QuadComponent.hpp"


namespace leon
{
	struct GridData
	{
		GridData() :
			sizeOfGrid(5, 5),
			gridSize(32,32)
		{
			background.texName = "overlay/white";
			background.layer = GraphicsLayer::OverlayBottom;
			gridSize = Vec2(32, 32);
			sizeOfGrid = Vec2(5, 5);
		}


		QuadComponentData background;
		Vec2 sizeOfGrid;
		Vec2 gridSize;
	};

	/// <summary>
	/// A grid for specific graphics objects.
	/// Designed primarily for the use of displaying the status of a ship (damage wise).
	/// </summary>
	class Grid
	{
	public:
		Grid(const GridData& data);
		virtual ~Grid();

		Vec2 getScreenPosition() const;
		void reset(const List<std::pair<String, sf::Vector2i> >& modules);
		void clear();

		/// <summary>
		/// Background texture of the grid
		/// </summary>
		sptr<QuadComponent> m_background;
	protected:
		void addHUDModule(const String& title, Vec2 shipModulePos);
		/// <summary>
		/// Given a grid position, indicate where the center of the new grid element should appear in pixels of screen coordinates.
		/// </summary>
		sf::Vector2f getPixelOffset(Vec2 gridPosition);

		struct GridElement
		{
			GridElement(const String& texName,
				const Vec2& gridSlot,
				const Vec2& gridSize
				)
			{
				gridPos = gridSlot;

				QuadComponentData data;
				data.layer = GraphicsLayer::OverlayMiddle;
				data.texName = texName;
				data.dimensions = static_cast<sf::Vector2f>(gridSize);
				module.reset(new QuadComponent(data));
			}

			sptr<QuadComponent> module;
			sptr<QuadComponent> damageIndicator;
			Vec2 gridPos;
		};


		/// <summary>
		/// The offset applied to grid elements to reset the 0,0. Defaulted as half the (size-1).
		/// </summary>
		Vec2 m_gridOffset;
		/// <summary>
		/// 1 unit of the grid is this many pixels
		/// </summary>
		Vec2 m_gridSize;

		List<GridElement> m_modules;
	};

}