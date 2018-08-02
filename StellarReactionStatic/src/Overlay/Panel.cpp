#include "Panel.hpp"
#include "globals.hpp"
#include "TextureAllocator.hpp"

using namespace leon;


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
	m_movesWithMouse = rData.movesWithMouse;
	f_assign(m_pPanel.get(), rData);

	m_pPanel->setBackgroundColor(rData.backgroundColor);
	if(rData.backgroundTex != "")
		m_pPanel->setBackgroundTexture(getGame()->getTextureAllocator().request(rData.backgroundTex));

	m_truePosition = getPosition();
}
void Panel::load(const String& fullFilePath)
{
	//Panels dont load stuff
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
void Panel::mouseMoveToPosition(sf::Vector2f pos)
{
	WidgetBase::mouseMoveToPosition(pos);

	for each(auto widget in m_widgetList)
	{
		widget->mouseMoveToPosition(pos);
	}
}


