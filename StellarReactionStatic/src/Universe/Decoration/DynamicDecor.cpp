#include "DynamicDecor.hpp"

using namespace std;

DynamicDecor::DynamicDecor(const DynamicDecorData& rData) : DecorQuad(rData, &m_quad), m_quad(rData.quadComp)
{

}
DynamicDecor::~DynamicDecor()
{

}
bool DynamicDecor::inputHook(std::string rCommand, sf::Packet rData)
{
	if (rCommand == "filler")
	{
		cout << "filler.";
		return true;
	}
	else
		return false;
}
