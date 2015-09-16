#ifndef WIDGETBASE_H
#define WIDGETBASE_H

#include "stdafx.hpp"
#include "Globals.hpp"
#include "IOComponent.hpp"

namespace leon
{
	struct WidgetBaseData
	{
		WidgetBaseData() :
			startHidden(false),
			configFile("TGUI/widgets/Black.conf"),
			screenCoords(game.getWindow().getSize().x / 2, game.getWindow().getSize().y / 2),
			size(128, 64),
			transparency(255),
			ioComp(game.getCoreIO())
		{
		}
		bool startHidden;/**should this widget start invisible**/
		std::string configFile;/**TGUI config file**/
		sf::Vector2f screenCoords;/**upper left corner**/
		sf::Vector2f size;/**pixels**/
		unsigned char transparency;

		IOComponentData ioComp;
	};

	class WidgetBase
	{
	public:
		WidgetBase(const WidgetBaseData& rData);
		virtual ~WidgetBase();

		void enable();
		void disable();
		void show();
		void hide();
		
		void toggleHidden(bool hidden);
		void toggleEnabled(bool enabled);

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

		virtual void mouseEnteredHook();
		virtual void mouseLeftHook();
		virtual void mouseClickedHook();
		virtual void leftMousePressedHook();
		virtual void leftMouseReleasedHook();

		virtual void triggerHook();

	private:
		void input(const std::string rCommand, sf::Packet rData);
		void f_callback(const tgui::Callback& callback);

		bool m_startHidden;
		tgui::Widget* m_pWidget;
		unsigned char m_tempTransparency;
	};
}
#endif // WIDGETBASE_H
