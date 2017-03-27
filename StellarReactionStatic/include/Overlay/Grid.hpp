#pragma once
#include "Panel.hpp"


namespace leon
{
	struct PictureData;

	struct GridData : public PanelData
	{
		GridData() :
			PanelData(),
			sizeOfGrid(sf::Vector2i(7, 7))
		{
			gridSize = sf::Vector2i(32, 32);
		}

		sf::Vector2i sizeOfGrid;
	};

	/// <summary>
	/// A grid for specific graphics objects.
	/// </summary>
	class Grid : public Panel
	{
	public:
		Grid(tgui::Gui& gui, const GridData& data);
		Grid(tgui::Container& container, const GridData& data);
		~Grid();
		void f_initialize(const GridData& rData);


		virtual void add(const PictureData& data);
		void addSimple(const String& title, sf::Vector2i shipModulePos);
	protected:
		/// <summary>
		/// The offset applied to grid elements to reset the 0,0. Defaulted as half the size.
		/// </summary>
		sf::Vector2i gridOffset;
		void addModuleToGrid(const String& title, sf::Vector2i shipModulePos);
		virtual bool inputHook(const String rCommand, sf::Packet rData);
	};

}