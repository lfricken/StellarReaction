#include "Store.hpp"

using namespace leon;

Store::Store(tgui::Gui& gui, const StoreData& data) : Panel(gui, data)
{

}
Store::Store(tgui::Container& container, const StoreData& data) : Panel(container, data)
{

}
Store::~Store()
{

}
