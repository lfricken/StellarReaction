#pragma once
#include "QuadComponent.hpp"
#include "Health.hpp"

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

		/// <summary>
		/// Background art for the grid.
		/// </summary>
		QuadComponentData background;
		/// <summary>
		/// Size of grid in cells.
		/// </summary>
		Vec2 sizeOfGrid;
		/// <summary>
		/// How big each cell is.
		/// </summary>
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
		/// Update HUD element with health
		/// </summary>
		void damageModule(Vec2 pos, HealthState state, float healthPercent, bool flashDamage);
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

		/// <summary>
		/// Used to put into a grid to show damage and module status.
		/// </summary>
		class GridElement
		{
		public:
			GridElement(const String& texName, const Vec2& gridSlot, const Vec2& gridSize);
			/// <summary>
			/// marks the module and destroyed
			/// </summary>
			void Grid::GridElement::markBroken();
			/// <summary>
			/// 0 to 1 percent
			/// </summary>
			void flashDamage(float percentHealthRemain, HealthState state);
			/// <summary>
			/// Sets the position of this element with respect to the screen coordinates.
			/// </summary>
			void setGuiPosition(sf::Vector2f screenPos);
			/// <summary>
			/// Our position in the grid
			/// </summary>
			Vec2 gridPos;


			sptr<QuadComponent> module;
			sptr<QuadComponent> damageIndicator;
		private:
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