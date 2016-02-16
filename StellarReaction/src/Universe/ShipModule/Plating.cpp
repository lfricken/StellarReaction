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
		stealthOn(true);
	}
}
