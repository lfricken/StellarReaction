#pragma once

#include "Button.hpp"
#include "Picture.hpp"

namespace leon
{
	class DragComponent;
	class DraggableSurface;

	/// \brief Data to initialize Draggable.
	struct DraggableData : public ButtonData
	{
		DraggableData() :
			ButtonData(),
			parentPtr(nullptr),
			myPanelOffset(0, 0)
		{
			this->configFile = "TGUI/widgets/Transparency.conf";
			buttonText = "";
			size = sf::Vector2f(64, 64);
			icon.texName = "menu/default_icon";
			icon.size = sf::Vector2f(64, 64);
		}

		String metaData;

		DraggableSurface* parentPtr;

		sf::Vector2f myPanelOffset;//position of our panel so we know how to place ourselves
		PictureData icon;
	};

	/// \brief Something that can be dragged around the screen.
	///
	/// Obtains mouse coordinates from DragComponent.
	class Draggable : public Button
	{
	public:
		Draggable(tgui::Gui& gui, const DraggableData& rData);
		Draggable(tgui::Container& container, const DraggableData& rData);
		virtual ~Draggable();

		/// <summary>
		/// Data this draggable holds. In this case, name of the module BP it represents.
		/// </summary>
		const String& getMetaData() const;
		/// Check whether the DraggableSurface already has a Draggable at that position before setting position.
		bool trySetGridPosition(const sf::Vector2i& rGridPos);
		/// <summary>
		/// Given the mouse position in screen coordinates, any DragComponents::isDragging == true objects get moved.
		/// </summary>
		void setPosition(const sf::Vector2f& rPos);

	protected:
		virtual bool callbackHook2(const tgui::Callback& callback);
		void dropped();

	private:
		/// <summary>
		/// Data this draggable holds. In this case, name of the module BP it represents.
		/// </summary>
		String m_metaData;
		/// <summary>
		/// Called by the draggable component when we are getting dragged around.
		/// </summary>
		void f_update(const sf::Vector2f& rPos);
		void f_initialize(const DraggableData& rData);
		sptr<DragComponent> m_spDrag;
		/// <summary>
		/// If we want to set this objects position in the panel, given a window coordinate,
		/// we need to be able to subtract out the panels position.
		/// </summary>
		sf::Vector2f m_parentPanelOffset;
		sptr<Picture> m_spPic;

		DraggableSurface* m_pParent;
	};
}
