#include "SelectableItem.hpp"
#include "TextureAllocator.hpp"
#include "Globals.hpp"

using namespace leon;

SelectableItem::SelectableItem(tgui::Gui& gui, const SelectableItemData& rData) : m_id(rData.id), m_pic(gui)
{
	f_initialize(rData, &gui, false);
}
SelectableItem::SelectableItem(tgui::Container& container, const SelectableItemData& rData) : m_id(rData.id), m_pic(container)
{
	f_initialize(rData, &container, true);
}
SelectableItem::~SelectableItem()
{

}
void SelectableItem::f_initialize(const SelectableItemData& rData, void* container, bool isContainer)
{
	m_pic->loadFromTexture(*getGame()->getTextureAllocator().request(rData.texName));
	m_pic->setPosition(rData.buttData.screenCoords);
	m_pic->setSize(rData.buttData.size.x, rData.buttData.size.y);


	for(auto it = rData.labelData.begin(); it != rData.labelData.end(); ++it)
	{
		if(isContainer)
			m_labels.push_back(sptr<tgui::Label::Ptr>(new tgui::Label::Ptr(*static_cast<tgui::Container*>(container))));
		else
			m_labels.push_back(sptr<tgui::Label::Ptr>(new tgui::Label::Ptr(*static_cast<tgui::Gui*>(container))));

		(*m_labels.back())->load(contentDir() + it->configFile);
		(*m_labels.back())->setText(it->text);
		(*m_labels.back())->setPosition(it->position);
		(*m_labels.back())->setAutoSize(true);
		(*m_labels.back())->setTextSize(it->textSize);
	}

	sf::Packet pack(rData.buttData.ioComp.courierList.back().message.getData());
	sf::Vector2i defaultPosition(0, 0);
	pack << rData.id;
	pack << defaultPosition.x;
	pack << defaultPosition.y;
	ButtonData data(rData.buttData);
	data.ioComp.courierList.back().message.setData(pack);

	if(isContainer)
		m_spButt = sptr<Button>(new Button(*static_cast<tgui::Container*>(container), data));
	else
		m_spButt = sptr<Button>(new Button(*static_cast<tgui::Gui*>(container), data));
	
}
