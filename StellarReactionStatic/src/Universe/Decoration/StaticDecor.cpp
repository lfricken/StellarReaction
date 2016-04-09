#include "StaticDecor.hpp"

using namespace std;

StaticDecor::StaticDecor(const StaticDecorData& rData) : DecorQuad(rData)
{
	m_parallax = rData.parallax;
	m_position = rData.position;
}
StaticDecor::~StaticDecor()
{

}

void StaticDecorData::loadJson(const Json::Value& root)
{
	GETJSON(parallax);
	GETJSON(position);

	DecorQuadData::loadJson(root);
}
bool StaticDecor::inputHook(std::string rCommand, sf::Packet rData)
{
	if (rCommand == "filler")
	{
		cout << "filler.";
		return true;
	}
	else
		return false;
}
