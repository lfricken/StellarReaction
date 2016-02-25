#include "Spinner.hpp"

#include "Convert.hpp"

using namespace std;

Spinner::Spinner(const SpinnerData& rData) : QuadComponent(rData)
{
	m_timer.getTimeElapsed();
	setRotationRate(rData.rate);
	m_on = true;

	if(!rData.startOn)
		toggleOn(false);

	if(rData.randomRot)
		m_permanentRot += rand()%7;

}
Spinner::~Spinner()
{

}
void Spinner::toggleOn(bool on)
{
	if(m_on == true && on == false)
	{
		cout << "\n" << on;
		m_oldRate = m_rate;
		m_rate = 0;
		m_on = false;
	}
	else if(m_on == false && on == true)
	{
		m_rate = m_oldRate;
		m_on = true;
	}
}
void Spinner::setRotationRate(float degCCW)
{
	m_rate = leon::degToRad(degCCW);
	m_oldRate = m_rate;
}
void Spinner::postUpdate()
{
	QuadComponent::postUpdate();
	m_permanentRot += m_timer.getTimeElapsed()*m_rate;
}
