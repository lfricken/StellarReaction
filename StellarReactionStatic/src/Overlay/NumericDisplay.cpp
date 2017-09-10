#include "NumericDisplay.hpp"
#include "Debugging.hpp"
#include "Picture.hpp"


using namespace leon;

NumericDisplay::NumericDisplay(tgui::Gui& gui, const NumericDisplayData& rData) : Panel(gui, rData)
{
	f_initialize(rData);
}
NumericDisplay::NumericDisplay(tgui::Container& container, const NumericDisplayData& rData) : Panel(container, rData)
{
	f_initialize(rData);
}
NumericDisplay::~NumericDisplay()
{

}
void NumericDisplay::f_initialize(const NumericDisplayData& rData)
{
	m_digitSize = rData.digitSize;

	m_pPanel->setSize(rData.digitSize.x * rData.numDigits, rData.digitSize.y);

	for(int i = 0; i < rData.numDigits; ++i)
	{
		PictureData picData;
		picData.size = sf::Vector2f(64, 128);
		int gridX = ((rData.numDigits - 1) - i);
		picData.screenCoords = sf::Vector2f(gridX*rData.digitSize.x, 0);
		add(sptr<Picture>(new Picture(*this->getPanelPtr(), picData)));
	}
}
void NumericDisplay::setNumber(int val)
{
	const String path = "numeric/";
	int digits = m_numbersList.size();

	for(int i = 0; i < digits; ++i)
	{
		int digit = val % 10;
		m_numbersList[i]->setTexture(path + String(digit));//set texture
		m_numbersList[i]->setSize(m_digitSize);//resize
		val /= 10;
	}
}
void NumericDisplay::add(sptr<WidgetBase> sp_widget)
{
	WidgetBase* widget = sp_widget.get();
	Picture* pic = dynamic_cast<Picture*>(widget);
	if(pic)
	{
		m_numbersList.push_back(pic);
		Panel::add(sp_widget);
	}
	else
	{
		WARNING;
	}
}
bool NumericDisplay::inputHook(const String rCommand, sf::Packet rData)
{
	if(rCommand == "setValue")
	{
		int32_t value;
		rData >> value;
		setNumber(value);
		return true;
	}
	else
		return false;
}