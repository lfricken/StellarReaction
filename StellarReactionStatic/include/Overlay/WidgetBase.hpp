#pragma once

#include "stdafx.hpp"
#include "Globals.hpp"
#include "IOComponent.hpp"
#include "NonCopyable.hpp"

/// A wrapper around TGUI Widgets.
namespace leon
{

	struct TooltipTextData
	{
		enum class Alignment
		{
			LeftOfMouse,
			RightOfMouse,
			CenterOfMouse,
		};

		TooltipTextData() :
			text(""),
			textPixelHeight(20),
			textColor(0,0,0,255),
			backColor(128,128,128,255),
			align(Alignment::RightOfMouse)
		{

		}

		String text;
		int textPixelHeight;
		sf::Color textColor;
		sf::Color backColor;
		Alignment align;


		void intoPacket(sf::Packet* data) const;
		void fromPacket(sf::Packet* data);
	};

	/// All data necessary to create a new Widget.
	struct WidgetBaseData
	{
		WidgetBaseData() :
			startHidden(false),
			//configFile("TGUI/widgets/Black.conf"),
			configFile("TGUI/widgets/NewMenu.conf"),
			screenCoords(-1, -1),
			size(128, 64),
			alpha(255),
			ioComp(&getGame()->getCoreIO()),
			gridSize(0, 0),
			movesWithMouse(false),
			percievedDistance(100),
			gridPosition(1, 1)
		{
		}
		bool startHidden; // should this widget start invisible
		String configFile; // TGUI config file

		TooltipTextData tooltip; // Tooltip to show on mouseover.

		unsigned char alpha; // 0 is fully transparent

		bool movesWithMouse; // true if this widget moves with the mouse
		float percievedDistance; // large (80+) for less movement with mouse

		sf::Vector2i gridSize;//how big are the grid snaps, THIS GETS SET IN DraggableSurface.cpp
		sf::Vector2i gridPosition;//if this isn't -1,-1, we will override our screen coordinates and set to this grid position
		sf::Vector2f screenCoords;/**upper left corner**/
		sf::Vector2f size;/**pixels**/

		IOComponentData ioComp;
	};

	/// Widget base class for all GUI items.
	class WidgetBase : NonCopyable
	{
	public:
		WidgetBase(tgui::Gui& gui, const WidgetBaseData& rData);
		WidgetBase(tgui::Container& rContainer, const WidgetBaseData& rData);
		void init(const WidgetBaseData& rData);
		virtual ~WidgetBase();

		/// Allow this Widget to receive events.
		void enable();
		/// Stop this Wdiget from receiving events.
		void disable();
		/// Make this Widget visible.
		void show();
		/// Make this Widget invisible (also disables it).
		virtual void hide();
		
		/// Toggle whether this widget is hidden.
		void toggleHidden(bool hidden);
		/// Toggle whether this widget is enabled.
		void toggleEnabled(bool enabled);

		void setSize(sf::Vector2f size);

		/// Set the position of this Widgets top right corner in window coordinates.
		virtual void setPosition(const sf::Vector2f& realPos);
		/// Return the position of this Widgets top right corner in window coordinates.
		virtual const sf::Vector2f& getPosition() const;

		virtual void mouseMoveToPosition(sf::Vector2f pos);

		void setGridPosition(sf::Vector2i gridPos);
		sf::Vector2i getGridPosition() const;
		sf::Vector2i getLastGridPosition() const;

		/// <summary>
		/// Get the nearest grid position for a particular real position.
		/// </summary>
		sf::Vector2i toGrid(sf::Vector2f realPos) const;
		sf::Vector2f fromGrid(sf::Vector2i gridPos) const;

		IOComponent m_io;
	protected:
		/// <summary>
		/// Force deriver to load config file.
		/// </summary>
		virtual void load(const String& fullFilePath) = 0;

		
		void f_assign(tgui::Widget* pWidget, const WidgetBaseData& rData);//must assign m_pWidget to something!


		/**events**/
		void f_MouseEntered();
		void f_MouseLeft();

		void f_LeftMouseClicked();
		void f_LeftMousePressed();
		void f_LeftMouseReleased();
		void f_RightMouseClicked();
		void f_RightMousePressed();
		void f_RightMouseReleased();

		void f_trigger();


		float m_percievedDistance; // large (80+) for less movement with mouse
		sf::Vector2f m_truePosition; // The intended position of this widget (could be offset by mouse movement).
		bool m_movesWithMouse; // True if this grid reacts to the mouse moving.
		sf::Vector2i m_gridSize; // Size of the grid in pixels.
		sf::Vector2i m_lastGridPosition; // Record the last position we were in so we can go back if dropped in a invalid location.
		bool m_onGrid; // If true, when setting the position

		/// <summary>
		/// Return true if the hook handled the call.
		/// </summary>
		virtual bool inputHook(const String rCommand, sf::Packet rData);
		virtual bool callbackHook(const tgui::Callback& callback);

		virtual void mouseEnteredHook(sf::Packet& rPack);
		virtual void mouseLeftHook(sf::Packet& rPack);
		virtual void leftMouseClickedHook(sf::Packet& rPack);
		virtual void leftMousePressedHook(sf::Packet& rPack);
		virtual void leftMouseReleasedHook(sf::Packet& rPack);

		virtual void triggerHook(sf::Packet& rPack);

	private:
		void input(const String rCommand, sf::Packet rData);
		void f_callback(const tgui::Callback& callback);

		tgui::Widget* m_pWidget;

		tgui::Container* pCon;
		tgui::Gui* pGui;
	};
}
