#ifndef WIDGETBASE_H
#define WIDGETBASE_H

#include "stdafx.hpp"
#include "Globals.hpp"
#include "IOComponent.hpp"

namespace leon
{
	struct WidgetBaseData
	{
		WidgetBaseData():
			startHidden(false),
			configFile("TGUI/widgets/Black.conf"),
			screenCoords(game.getWindow().getSize().x/2, game.getWindow().getSize().y/2),
			size(128,64),
			ioComp(game.getCoreIO())
		{
		}
		bool startHidden;/**should this widget start invisible**/
		std::string configFile;/**TGUI config file**/
		sf::Vector2f screenCoords;/**upper left corner**/
		sf::Vector2f size;/**pixels**/

		IOComponentData ioComp;
	};

	class WidgetBase
	{
	public:
		WidgetBase(const WidgetBaseData& rData);
		virtual ~WidgetBase();

		virtual void enable() final;
		virtual void disable() final;
		virtual void show() final;
		virtual void hide() final;

		virtual void toggleHidden(bool hidden) final;
		virtual void toggleEnabled(bool enabled) final;

	protected:
		virtual void input(const std::string rCommand, sf::Packet rData);
		void f_assign(tgui::Widget* pWidget);//must assign m_pWidget to something!

		virtual void f_callback(const tgui::Callback& callback);
		virtual void f_MouseEntered();
		virtual void f_MouseLeft();
		virtual void f_LeftMouseClicked();
		virtual void f_trigger();

		IOComponent m_io;
	private:
		bool m_startHidden;
		tgui::Widget* m_pWidget;
	};
}
#endif // WIDGETBASE_H
