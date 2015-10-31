#pragma once

#include "Button.hpp"
#include "Picture.hpp"

namespace leon
{
	class DragComponent;
	class DraggableSurface;

	struct DraggableData : public ButtonData
	{
		DraggableData() :
			ButtonData(),
			parentPtr(NULL),
			myPanelOffset(0, 0),
			gridSize(200, 200),
			gridPosition(-1, -1)
		{
			this->configFile = "TGUI/widgets/Transparency.conf";
			buttonText = "";
			size = sf::Vector2f(64, 64);
			icon.texName = "menu/default_icon.png";
			icon.size = sf::Vector2f(64, 64);
		}

		std::string metaData;

		DraggableSurface* parentPtr;
		sf::Vector2f gridPosition;//if this isn't -1,-1, we will override our screen coordinates and set to this grid position
		sf::Vector2f gridSize;//how big are the grid snaps, THIS GETS SET IN DraggableSurface.cpp
		sf::Vector2f myPanelOffset;//position of our panel so we know how to place ourselves
		PictureData icon;
	};


	class Draggable : public Button
	{
	public:
		Draggable(tgui::Gui& gui, const DraggableData& rData);
		Draggable(tgui::Container& container, const DraggableData& rData);
		virtual ~Draggable();

		sf::Vector2f getGridPosition() const;
		void setGridPosition(const sf::Vector2f& rPos);
		const std::string& getMetaData() const;
		bool trySetPosition(const sf::Vector2f& rPos);
		void setPosition(const sf::Vector2f& rPos);

	protected:
		virtual bool callbackHook2(const tgui::Callback& callback);

		void dropped();
		sf::Vector2f getClosestPosition(const sf::Vector2f& rCurrent);

	private:
		std::string m_metaData;//data this draggable holds
		void f_update(const sf::Vector2f& rPos);
		void f_initialize(const DraggableData& rData);
		sptr<DragComponent> m_spDrag;
		sf::Vector2f m_parentPanelOffset;
		sf::Vector2f m_gridSize;
		sptr<Picture> m_spPic;

		DraggableSurface* m_pParent;
		sf::Vector2f m_lastPosition;
	};
}
