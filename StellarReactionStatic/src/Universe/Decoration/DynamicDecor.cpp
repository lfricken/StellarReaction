#include "DynamicDecor.hpp"

using namespace std;

DynamicDecor::DynamicDecor(const DynamicDecorData& rData) : DecorQuad(rData)
{
	m_parallax = rData.parallax;
	m_frequency = rData.frequency;
	m_minX = rData.minX;
	m_minY = rData.minY;
	m_maxX = rData.maxX;
	m_maxY = rData.maxY;
}
DynamicDecor::~DynamicDecor()
{

}

void DynamicDecorData::loadJson(const Json::Value& root)
{
	GETJSON(parallax);
	GETJSON(frequency);
	GETJSON(minX);
	GETJSON(minY);
	GETJSON(maxX);
	GETJSON(maxY);

	DecorQuadData::loadJson(root);
}
bool DynamicDecor::inputHook(string rCommand, sf::Packet rData)
{
	if (rCommand == "filler")
	{
		cout << "filler.";
		return true;
	}
	else
		return false;
}