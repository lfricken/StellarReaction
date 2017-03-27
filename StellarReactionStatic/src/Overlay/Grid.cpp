#include "Grid.hpp"
#include "Picture.hpp"
#include "ShipModule.hpp"
#include "Globals.hpp"
#include "BlueprintLoader.hpp"

namespace leon
{
	Grid::Grid(tgui::Gui& container, const GridData& data) : Panel(container, data)
	{
		f_initialize(data);
	}
	Grid::Grid(tgui::Container& container, const GridData& data) : Panel(container, data)
	{
		f_initialize(data);
	}
	Grid::~Grid()
	{

	}
	void Grid::f_initialize(const GridData& data)
	{
		m_io.getName();

		gridOffset.x = (data.sizeOfGrid.x - 1) / 2;
		gridOffset.y = (data.sizeOfGrid.y - 1) / 2;

		m_pPanel->setSize((float)data.sizeOfGrid.x * m_gridSize.x, (float)data.sizeOfGrid.y* m_gridSize.y);

		m_pPanel->disable();//so it doesn't get in our way in terms of gui events and clicking
		addSimple("", sf::Vector2i());
	}
	void Grid::addSimple(const String& title, sf::Vector2i shipModulePos)
	{
		PictureData data;
		data.gridSize = this->m_gridSize;
		data.gridPosition = sf::Vector2i(3,0);
		data.size = (sf::Vector2f)data.gridSize;


		this->add(data);
		data.gridPosition.x -= 1;
		this->add(data);
		data.gridPosition.x -= 1;
		this->add(data);
		data.gridPosition.x -= 1;
		this->add(data);
		data.gridPosition.x -= 1;
		this->add(data);
		data.gridPosition.x -= 1;
		this->add(data);
		data.gridPosition.x -= 1;
		this->add(data);
	}
	void Grid::add(const PictureData& dataOrig)
	{
		auto data = dataOrig;
		data.gridPosition += gridOffset;
		sptr<Picture> graphic(new Picture(*this->getPanelPtr(), data));
		Panel::add(graphic);
	}
	bool Grid::inputHook(const String rCommand, sf::Packet data)
	{
		if(rCommand == "damagedModule")
		{
			sf::Vector2i gridPos;
			data >> gridPos.x;
			data >> gridPos.y;

			std::cout << "\n" << gridPos.x << gridPos.y << " hit";

			return true;
		}
		else if(rCommand == "addModuleToGrid")//setState
		{
			String title;
			sf::Vector2i shipModulePos;

			data >> title;
			data >> shipModulePos.x;
			data >> shipModulePos.y;

			addModuleToGrid(title, shipModulePos);

			return true;
		}
		else
			return false;
	}
	void Grid::addModuleToGrid(const String& title, sf::Vector2i shipModulePos)
	{
		sptr<ShipModuleData> pNewModuleData = sptr<ShipModuleData>(dynamic_cast<ShipModuleData*>(game.getUniverse().getBlueprints().getModuleSPtr(title)->clone()));

		PictureData data;
		data.gridSize = this->m_gridSize;
		data.gridPosition = shipModulePos;
		data.texName = pNewModuleData->baseDecor.texName;
		data.size = (sf::Vector2f)data.gridSize;

		this->add(data);
	}
}