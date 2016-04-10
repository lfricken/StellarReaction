#include "StaticDecor.hpp"

using namespace std;

StaticDecor::StaticDecor(const StaticDecorData& rData) : DecorQuad(rData)
{
}
StaticDecor::~StaticDecor()
{

}

void StaticDecorData::loadJson(const Json::Value& root)
{

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
