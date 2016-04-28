#pragma once

#include "stdafx.hpp"
#include "Globals.hpp"
#include "IOComponent.hpp"
#include "NonCopyable.hpp"

namespace leon
{
	struct WidgetBaseData
	{
		WidgetBaseData() :
			startHidden(false),
			//configFile("TGUI/widgets/Black.conf"),
			screenCoords(5, 5),
			size(128, 64),
			transparency(255),
			ioComp(&game.getCoreIO())
		{
		}
		bool startHidden;/**should this widget start invisible**/
		std::string configFile;/**TGUI config file**/
		sf::Vector2f screenCoords;/**upper left corner**/
		sf::Vector2f size;/**pixels**/
		unsigned char transparency;

		IOComponentData ioComp;
	};

	class WidgetBase : NonCopyable
	{
	public:
		WidgetBase(tgui::Gui& gui, const WidgetBaseData& rData);
		WidgetBase(tgui::Container& rContainer, const WidgetBaseData& rData);
		virtual ~WidgetBase();

		void enable();
		void disable();
		void show();
		void hide();
		
		void toggleHidden(bool hidden);
		void toggleEnabled(bool enabled);

		virtual void setPosition(const sf::Vector2f& newPos);
		virtual const sf::Vector2f& getPosition() const;

	protected:
		IOComponent m_io;
		void f_assign(tgui::Widget* pWidget);//must assign m_pWidget to something!


		/**events**/
		void f_MouseEntered();
		void f_MouseLeft();
		void f_LeftMouseClicked();
		void f_LeftMousePressed();
		void f_LeftMouseReleased();

		void f_trigger();



		/**events HOOKS**/
		virtual bool inputHook(const std::string rCommand, sf::Packet rData);
		virtual bool callbackHook(const tgui::Callback& callback);

		virtual void mouseEnteredHook(sf::Packet& rPack);
		virtual void mouseLeftHook(sf::Packet& rPack);
		virtual void mouseClickedHook(sf::Packet& rPack);
		virtual void leftMousePressedHook(sf::Packet& rPack);
		virtual void leftMouseReleasedHook(sf::Packet& rPack);

		virtual void triggerHook(sf::Packet& rPack);

	private:
		void input(const std::string rCommand, sf::Packet rData);
		void f_callback(const tgui::Callback& callback);

		bool m_startHidden;
		tgui::Widget* m_pWidget;
		unsigned char m_tempTransparency;

		tgui::Container* pCon;
		tgui::Gui* pGui;
	};
}
