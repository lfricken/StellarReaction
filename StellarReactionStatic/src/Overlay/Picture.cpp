#include "Picture.hpp"
#include "Globals.hpp"
#include "TextureAllocator.hpp"

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
	m_pPicture->loadFromTexture(*game.getTextureAllocator().request(rData.texName));
	f_assign(m_pPicture.get(), rData);
}
void Picture::load(const String& fullFilePath)
{
	//does nothing because we need to load manually
}

