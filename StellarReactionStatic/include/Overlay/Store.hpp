#pragma once

#include "Panel.hpp"

namespace leon
{
	struct StoreData : PanelData
	{


	};

	class Store : public Panel
	{
	public:
		Store(tgui::Gui& gui, const StoreData& data);
		Store(tgui::Container& container, const StoreData& data);
		virtual ~Store();


	};

}
