#include "Picture.hpp"
#include "globals.hpp"

namespace leon
{
	Picture::Picture(tgui::Gui& rGui, const PictureData& rData) : WidgetBase(rData), m_pPicture(rGui)
	{
		f_initialize(rData);
	}
	Picture::Picture(tgui::Container& rContainer, const PictureData& rData) : WidgetBase(rData), m_pPicture(rContainer)
	{
		f_initialize(rData);
	}
	Picture::~Picture()
	{

	}
	void Picture::f_initialize(const PictureData& rData)
	{
		f_assign(m_pPicture.get());
		m_pPicture->load("textures/"+rData.texName);
		m_pPicture->setPosition(rData.screenCoords);
		m_pPicture->setSize(rData.size.x, rData.size.y);

		m_pPicture->bindCallbackEx(&Picture::f_callback, this, tgui::Picture::AllPictureCallbacks);
	}

	void Picture::f_callback(const tgui::Callback& callback)
	{

	}




}
