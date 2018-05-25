#include "Spinner.hpp"

#include "Convert.hpp"
#include "JSON.hpp"

void SpinnerData::loadJson(const Json::Value& root)
{
	QuadComponentData::loadJson(root);

	GETJSON(rate);
	GETJSON(randomRot);
	GETJSON(startOn);
}
Spinner::Spinner(const SpinnerData& rData) : QuadComponent(rData)
{
	m_timer.getTimeElapsed();
	setRotationRate(rData.rate);

	if(rData.randomRot)
		m_permanentRot += Rand::get(0.f, Math::Tau);

	toggleOn(rData.startOn);
}
Spinner::~Spinner()
{

}
void Spinner::toggleOn(bool on)
{
	m_on = on;
}
void Spinner::setRotationRate(float degCCW)
{
	m_rate = Convert::degToRad(degCCW);
}
void Spinner::postUpdate()
{
	QuadComponent::postUpdate();
	if(m_on)
		m_permanentRot += m_timer.getTimeElapsed()*m_rate;
}
