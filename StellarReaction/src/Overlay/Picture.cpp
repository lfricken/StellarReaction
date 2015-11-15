#include "Picture.hpp"
#include "Globals.hpp"

using namespace leon;

Picture::Picture(tgui::Gui& rGui, const PictureData& rData) : WidgetBase(rGui, rData), m_pPicture(rGui)
{
	f_initialize(rData);
}
Picture::Picture(tgui::Container& rContainer, const PictureData& rData) : WidgetBase(rContainer, rData), m_pPicture(rContainer)
{
	f_initialize(rData);
}
Picture::~Picture()
{

}
void Picture::f_initialize(const PictureData& rData)
{
	f_assign(m_pPicture.get());
	m_pPicture->load(contentDir() + "textures/" + rData.texName);
	m_pPicture->setPosition(rData.screenCoords);
	m_pPicture->setSize(rData.size.x, rData.size.y);
}
