#include "StaticDecor.hpp"

using namespace std;

StaticDecor::StaticDecor(const StaticDecorData& rData) : DecorQuad(rData, &m_quad), m_quad(rData.quadComp)
{

}
StaticDecor::~StaticDecor()
{

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
