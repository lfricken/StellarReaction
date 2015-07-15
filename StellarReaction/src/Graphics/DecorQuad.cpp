#include "DecorQuad.hpp"

using namespace std;

DecorQuad::DecorQuad(const DecorQuadData& rData) : Decoration(rData, &m_quad), m_quad(rData.quadComp)
{

}
DecorQuad::~DecorQuad()
{

}
void DecorQuad::input(std::string rCommand, sf::Packet rData)
{
	if(rCommand == "filler")
	{
		cout << "filler.";
	}
	else
		Decoration::input(rCommand, rData);
}
