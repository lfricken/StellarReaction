#include "Grid.hpp"
#include "Picture.hpp"

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
		gridOffset.x = (data.sizeOfGrid.x - 1) / 2;
		gridOffset.y = (data.sizeOfGrid.y - 1) / 2;

		m_pPanel->setSize((float)data.sizeOfGrid.x * m_gridSize.x, (float)data.sizeOfGrid.y* m_gridSize.y);

		addSimple("", sf::Vector2i());
	}
	void Grid::addSimple(const String& title, sf::Vector2i shipModulePos)
	{
		PictureData data;
		data.gridSize = this->m_gridSize;
		data.gridPosition.x = 3;
		data.gridPosition.y = 0;
		data.size.x = 64;
		data.size.y = 64;


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
}