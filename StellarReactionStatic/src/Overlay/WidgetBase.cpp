#include "WidgetBase.hpp"
#include "Debugging.hpp"

using namespace leon;

void TooltipTextData::intoPacket(sf::Packet* dataPtr) const
{
	auto& data = *dataPtr;
	uint32_t alignInt = (int)align;

	data << text;
	data << textPixelHeight;
	data << textColor.toInteger();
	data << backColor.toInteger();
	data << alignInt;
}
void TooltipTextData::fromPacket(sf::Packet* dataPtr)
{
	auto& data = *dataPtr;
	uint32_t colorInt;
	uint32_t backColorInt;
	uint32_t alignInt;

	data >> text;
	data >> textPixelHeight;
	data >> colorInt;
	data >> backColorInt;
	data >> alignInt;

	textColor = sf::Color(colorInt);
	backColor = sf::Color(backColorInt);
	align = (Alignment)alignInt;
}

WidgetBase::WidgetBase(tgui::Gui& gui, const WidgetBaseData& rData) : m_io(rData.ioComp, &leon::WidgetBase::input, this)
{
	pCon = nullptr;
	pGui = &gui;
	init(rData);
}
WidgetBase::WidgetBase(tgui::Container& rContainer, const WidgetBaseData& rData) : m_io(rData.ioComp, &leon::WidgetBase::input, this)
{
	pCon = &rContainer;
	pGui = nullptr;
	init(rData);
}
void WidgetBase::init(const WidgetBaseData& rData)
{
	m_movesWithMouse = rData.movesWithMouse;
	m_percievedDistance = rData.percievedDistance;

	if(rData.tooltip.text != "")//if not empty
	{
		sf::Packet tooltipData;
		rData.tooltip.intoPacket(&tooltipData);

		Courier tooltip;
		tooltip.condition.reset(EventType::MouseEntered, 0, 'd', true);
		tooltip.message.reset("tooltip", "setTooltip", tooltipData, 0, false);
		m_io.getEventer().add(tooltip);

		Courier untooltip;
		untooltip.condition.reset(EventType::MouseLeft, 0, 'd', true);
		untooltip.message.reset("tooltip", "unsetTooltip", voidPacket, 0, false);
		m_io.getEventer().add(untooltip);
	}
}
void WidgetBase::f_assign(tgui::Widget* pWidget, const WidgetBaseData& rData)
{
	//Config file
	load(contentDir() + rData.configFile);

	//Start Hidden
	m_pWidget = pWidget;
	if(rData.startHidden)
		m_pWidget->hide();

	//Transparency
	m_pWidget->setTransparency(rData.alpha);

	//Grid Size
	m_gridSize = rData.gridSize;
	
	//If we are using grid coordinates
	if(rData.screenCoords == sf::Vector2f(-1,-1))
		setGridPosition(rData.gridPosition);
	else//Screen Position
		setPosition(rData.screenCoords);

	//Size
	m_pWidget->setSize(rData.size.x, rData.size.y);

	//Bind for events
	int bindAllMaskValue = 4095;//TODO: allthough this is a lot of 1's, why not have more 1's? Where did this value come from? todo probably anding of tgui enums
	m_pWidget->bindCallbackEx(&WidgetBase::f_callback, this, bindAllMaskValue);

}
WidgetBase::~WidgetBase()
{
	if(pCon != nullptr)
		pCon->remove(m_pWidget);
}
void WidgetBase::enable()
{
	m_pWidget->enable();
}
void WidgetBase::disable()
{
	m_pWidget->disable();
}
void WidgetBase::show()
{
	m_pWidget->show();
	m_pWidget->moveToFront();
}
void WidgetBase::hide()
{
	m_pWidget->hide();
}
void WidgetBase::toggleHidden(bool hidden)
{
	if(hidden)
	{
		hide();

		Message message;
		message.reset("tooltip", "unsetTooltip", voidPacket, 0, false);
		getGame()->getCoreIO().recieve(message);
	}
	else
		show();
}
void WidgetBase::toggleEnabled(bool enabled)
{
	if(enabled)
		enable();
	else
		disable();
}
void WidgetBase::setSize(sf::Vector2f size)
{
	m_pWidget->setSize(size.x, size.y);
}
void WidgetBase::setPosition(const sf::Vector2f& realPos)
{
	m_truePosition = realPos;
	m_pWidget->setPosition(m_truePosition);
}
void WidgetBase::mouseMoveToPosition(sf::Vector2f pos)
{
	if(m_movesWithMouse)
	{
		auto size = (sf::Vector2f)getGame()->getWindow().getSize();
		size.x /= 2;
		size.y /= 2;
		auto positionOffset = pos - size;
		positionOffset.x /= m_percievedDistance;
		positionOffset.y /= m_percievedDistance;
		m_pWidget->setPosition(m_truePosition - positionOffset);
	}
}
const sf::Vector2f& WidgetBase::getPosition() const
{
	return m_pWidget->getPosition();
}
void WidgetBase::setGridPosition(sf::Vector2i gridPos)
{
	gridPos.x *= m_gridSize.x;
	gridPos.y *= m_gridSize.y;

	setPosition((sf::Vector2f)gridPos);
	m_lastGridPosition = getGridPosition();
}
sf::Vector2i WidgetBase::getGridPosition() const
{
	sf::Vector2f gridPos = getPosition();
	gridPos.x /= m_gridSize.x;
	gridPos.y /= m_gridSize.y;
	return (sf::Vector2i)gridPos;
}
sf::Vector2i WidgetBase::getLastGridPosition() const
{
	return m_lastGridPosition;
}
sf::Vector2i WidgetBase::toGrid(sf::Vector2f realPos) const
{
	sf::Vector2f gridHalf(m_gridSize.x / 2.f, m_gridSize.y / 2.f);
	sf::Vector2f calculatedReal = (realPos + gridHalf);
	sf::Vector2i newGridPos;
	newGridPos.x = (int)calculatedReal.x / m_gridSize.x;
	newGridPos.y = (int)calculatedReal.y / m_gridSize.y;

	return newGridPos;
}
sf::Vector2f WidgetBase::fromGrid(sf::Vector2i gridPos) const
{
	return sf::Vector2f((float)(gridPos.x * m_gridSize.x), (float)(gridPos.y * m_gridSize.y));
}
/**IO**/
void WidgetBase::input(String rCommand, sf::Packet rData)
{
	if(inputHook(rCommand, rData))
	{

	}
	else if(rCommand == "toggleHidden")
	{
		toggleHidden(m_pWidget->isVisible());
	}
	else if(rCommand == "setHidden")
	{
		bool mode = false;
		rData >> mode;
		toggleHidden(mode);
	}
	else if(rCommand == "toggleEnabled")
	{
		toggleEnabled(!m_pWidget->isEnabled());
	}
	else if(rCommand == "setEnabled")
	{
		bool mode = false;
		rData >> mode;
		toggleEnabled(mode);
	}
	else if(rCommand == "trigger")
	{
		f_trigger();
	}
	else
	{
		Print << "\nCommand [" << rCommand << "] not found." << FILELINE;
		///ERROR
	}
}
void WidgetBase::f_callback(const tgui::Callback& callback)
{
	if(callbackHook(callback))
	{

	}
	else if(callback.trigger == tgui::Widget::WidgetCallbacks::MouseEntered)
	{
		f_MouseEntered();
	}
	else if(callback.trigger == tgui::Widget::WidgetCallbacks::MouseLeft)
	{
		f_MouseLeft();
	}
	else if(callback.trigger == tgui::ClickableWidget::LeftMouseClicked)
	{
		f_LeftMouseClicked();
	}
	else if(callback.trigger == tgui::ClickableWidget::LeftMousePressed)
	{
		f_LeftMousePressed();
	}
	else if(callback.trigger == tgui::ClickableWidget::LeftMouseReleased)
	{
		f_LeftMouseReleased();
	}
	else if(callback.trigger == tgui::ClickableWidget::RightMouseClicked)
	{
		f_RightMouseClicked();
	}
	else if(callback.trigger == tgui::ClickableWidget::RightMousePressed)
	{
		f_RightMousePressed();
	}
	else if(callback.trigger == tgui::ClickableWidget::RightMouseReleased)
	{
		f_RightMouseReleased();
	}
}
void WidgetBase::f_MouseEntered()
{
	sf::Packet pack;
	mouseEnteredHook(pack);
	m_io.event(EventType::MouseEntered, 0, pack);
}
void WidgetBase::f_MouseLeft()
{
	sf::Packet pack;
	mouseLeftHook(pack);
	m_io.event(EventType::MouseLeft, 0, pack);
}
void WidgetBase::f_LeftMouseClicked()
{
	sf::Packet pack;
	leftMouseClickedHook(pack);
	m_io.event(EventType::LeftMouseClicked, 0, pack);
}
void WidgetBase::f_LeftMousePressed()
{
	sf::Packet pack;
	leftMousePressedHook(pack);
	m_io.event(EventType::LeftMousePressed, 0, pack);
}
void WidgetBase::f_LeftMouseReleased()
{
	sf::Packet pack;
	leftMouseReleasedHook(pack);
	m_io.event(EventType::LeftMouseReleased, 0, pack);
}
void WidgetBase::f_RightMouseClicked()
{
	sf::Packet pack;
	m_io.event(EventType::RightMouseClicked, 0, pack);
}
void WidgetBase::f_RightMousePressed()
{
	sf::Packet pack;
	m_io.event(EventType::RightMousePressed, 0, pack);
}
void WidgetBase::f_RightMouseReleased()
{
	sf::Packet pack;
	m_io.event(EventType::RightMouseReleased, 0, pack);
}
void WidgetBase::f_trigger()
{
	sf::Packet pack;
	triggerHook(pack);
	m_io.event(EventType::Triggered, 0, pack);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// Hooks
/// </summary>
bool WidgetBase::inputHook(const String rCommand, sf::Packet rData)
{
	return false;
}
bool WidgetBase::callbackHook(const tgui::Callback& callback)
{
	return false;
}
void WidgetBase::mouseEnteredHook(sf::Packet& rPack)
{
	rPack << m_io.getName();
}
void WidgetBase::mouseLeftHook(sf::Packet& rPack)
{
	rPack << m_io.getName();
}
void WidgetBase::leftMouseClickedHook(sf::Packet& rPack)
{
	rPack << m_io.getName();
}
void WidgetBase::leftMousePressedHook(sf::Packet& rPack)
{
	rPack << m_io.getName();
}
void WidgetBase::leftMouseReleasedHook(sf::Packet& rPack)
{
	rPack << m_io.getName();
}
void WidgetBase::triggerHook(sf::Packet& rPack)
{
	rPack << m_io.getName();
}


