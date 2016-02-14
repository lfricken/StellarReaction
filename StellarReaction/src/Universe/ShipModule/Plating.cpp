#include "Plating.hpp"
#include "Controller.hpp"

using namespace std;

Plating::Plating(const PlatingData& rData) : ShipModule(rData)
{

}
Plating::~Plating()
{

}

void Plating::directive(map<Directive, bool>& rIssues)
{
	if (rIssues[Directive::Stealth])
	{
		//turn on stealth mode
		stealthOn();
	}
}

void Plating::stealthOn() 
{
	sf::Color color = m_parentChunk->getHull()->getColor();
	//TODO: Make a constant for the alpha value of 50
	m_parentChunk->getHull()->setColor(sf::Color(color.r, color.g, color.b, 50));
}