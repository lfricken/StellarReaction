#include "DecorQuad.hpp"

using namespace std;

DecorQuad::DecorQuad(const DecorQuadData& rData) : Decoration(rData, &m_quad), m_title(rData.title), m_quad(rData.quadComp)
{

}
DecorQuad::~DecorQuad()
{

}

void DecorQuadData::loadJson(const Json::Value& root)
{
	LOADJSON(quadComp);
	GETJSON(title);

	DecorationData::loadJson(root);
}
bool DecorQuad::inputHook(std::string rCommand, sf::Packet rData)
{
	if(rCommand == "filler")
	{
		cout << "filler.";
		return true;
	}
	else
		return false;
}
