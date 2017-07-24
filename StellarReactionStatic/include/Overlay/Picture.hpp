#pragma once

#include "WidgetBase.hpp"

namespace leon
{
	/// Data to initialize a Picture.
	struct PictureData : public WidgetBaseData
	{
		PictureData() :
			WidgetBaseData(),
			texName("default")
		{
		}
		String texName;
	};

	/// A picture to display on the GUI.
	class Picture : public WidgetBase
	{
	public:
		Picture(tgui::Gui& rGui, const PictureData& rData);
		Picture(tgui::Container& rContainer, const PictureData& rData);
		virtual ~Picture();

		void setTexture(String texName);

	protected:
		void load(const String& fullFilePath);
	private:
		void f_initialize(const PictureData& data);

		tgui::Picture::Ptr m_pPicture;//pointer to our button in the gui
	};
}
