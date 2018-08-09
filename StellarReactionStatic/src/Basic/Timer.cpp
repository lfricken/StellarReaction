#include "Timer.hpp"
#include "globals.hpp"

#include "Universe.hpp"

Timer::Timer()
{
	clock = nullptr;

	m_lastTimeElapsed = getTime();
	m_countDownTime = 0.f;
	m_timeUpTime = getTime() + m_countDownTime;
}
Timer::Timer(float time)
{
	clock = nullptr;

	m_lastTimeElapsed = time;
	m_countDownTime = 0.f;
	m_timeUpTime = time + m_countDownTime;
}
Timer::Timer(const Clock& clockRef)
{
	clock = &clockRef;

	m_lastTimeElapsed = getTime();
	m_countDownTime = 0.f;
	m_timeUpTime = getTime() + m_countDownTime;
}
Timer::~Timer()
{

}

float Timer::getTimePercentageElapsed() const
{
	if(m_countDownTime <= 0)
		return 1.f;

	float startTime = m_timeUpTime - m_countDownTime;
	float time = getTime();
	float percentage = (time - startTime) / m_countDownTime;

	if(percentage >= 1.f)
		return 1.f;
	else if(percentage < 0.f)
		return 0.f;
	else
		return percentage;
}
float Timer::getTime() const//time elapsed as seconds since universe was initialized
{
	if(clock != nullptr)
		return clock->getTime();
	else
		return getGame()->getUniverse().getTime();
}
float Timer::getTimeElapsed()//get time elapsed since we last called this function
{
	float oldTime = m_lastTimeElapsed;
	m_lastTimeElapsed = getTime();
	return m_lastTimeElapsed - oldTime;
}
bool Timer::isTimeUp() const
{
	return getTime() > m_timeUpTime;
}
void Timer::setCountDown(float countDown)
{
	m_countDownTime = countDown;
}
void Timer::restartCountDown()
{
	m_timeUpTime = getTime() + m_countDownTime;
}
