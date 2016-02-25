#pragma once

#include "WidgetBase.hpp"

namespace leon
{

	struct PictureData : public WidgetBaseData
	{
		PictureData() :
			WidgetBaseData(),
			texName("default.png")
		{
		}
		std::string texName;
	};

	class Picture : public WidgetBase
	{
	public:
		Picture(tgui::Gui& rGui, const PictureData& rData);
		Picture(tgui::Container& rContainer, const PictureData& rData);
		virtual ~Picture();

	protected:

	private:
		void f_initialize(const PictureData& data);

		tgui::Picture::Ptr m_pPicture;//pointer to our button in the gui
	};
}
