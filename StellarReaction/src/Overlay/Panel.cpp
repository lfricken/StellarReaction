#include "Panel.hpp"
#include "globals.hpp"
#include "TextureAllocator.hpp"

using namespace leon;
using namespace std;

Panel::Panel(tgui::Gui& gui, const PanelData& rData) : WidgetBase(gui, rData), m_pPanel(gui, rData.ioComp.name)
{
	f_initialize(rData);
}
Panel::Panel(tgui::Container& container, const PanelData& rData) : WidgetBase(container, rData), m_pPanel(container, rData.ioComp.name)
{
	f_initialize(rData);
}
Panel::~Panel()
{

}
void Panel::f_initialize(const PanelData& rData)
{
	f_assign(m_pPanel.get());
	m_pPanel->setPosition(rData.screenCoords);
	m_pPanel->setSize(rData.size.x, rData.size.y);
	m_pPanel->setBackgroundColor(rData.backgroundColor);
	if(rData.backgroundTex != "")
		m_pPanel->setBackgroundTexture(game.getTextureAllocator().request(rData.backgroundTex));
}
tgui::Panel::Ptr Panel::getPanelPtr() const
{
	return m_pPanel;
}
void Panel::add(sptr<WidgetBase> sp_widget)
{
	m_widgetList.push_back(sp_widget);
}
void Panel::clear()//destroys all draggables
{
	m_widgetList.clear();
}
