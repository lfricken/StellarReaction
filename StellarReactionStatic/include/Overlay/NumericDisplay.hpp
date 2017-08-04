#pragma once

#include "Panel.hpp"
#include "Picture.hpp"
#include "stdafx.hpp"

namespace leon
{
	/// <summary>
	/// Displays images to show a number
	/// </summary>
	struct NumericDisplayData : public PanelData
	{
		NumericDisplayData() :
			numDigits(3),
			digitSize(32,64)
		{
			backgroundColor = sf::Color(0, 0, 0, 0);
		}
		int numDigits;
		sf::Vector2f digitSize;
	};

	/// <summary>
	/// Displays images to show a number
	/// </summary>
	class NumericDisplay : public Panel
	{
	public:
		NumericDisplay(tgui::Gui& gui, const NumericDisplayData& data);
		NumericDisplay(tgui::Container& container, const NumericDisplayData& data);
		~NumericDisplay();

		void setNumber(int val);

	protected:
		virtual bool inputHook(const String rCommand, sf::Packet rData);
		virtual void add(sptr<WidgetBase> sp_widget);

	private:
		void f_initialize(const NumericDisplayData& rData);
		sf::Vector2f m_digitSize;
		List<Picture*> m_numbersList;
	};
}
