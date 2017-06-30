#include "Timer.hpp"
#include "globals.hpp"

#include "Universe.hpp"

Timer::Timer()
{
	m_lastElapsedTime = getTime();
	m_countDownTime = 1.f;
	m_timeUpTime = getTime() + m_countDownTime;
}
Timer::Timer(float time)
{
	m_lastElapsedTime = time;
	m_countDownTime = 1.f;
	m_timeUpTime = time + m_countDownTime;
}
Timer::~Timer()
{

}

float Timer::getTimePercentageElapsed() const
{
	float startTime = m_timeUpTime - m_countDownTime;
	float percentage = (getTime() - startTime) / m_countDownTime;

	if(percentage > 1)
		return 1.f;
	else if(percentage < 0)
		return 0.f;
	else
		return percentage;
}
float Timer::getTime() const//time elapsed as seconds since universe was initialized
{
	return game.getUniverse().getTime();
}
float Timer::getTimeElapsed()//get time elapsed since we last called this function
{
	float oldTime = m_lastElapsedTime;
	m_lastElapsedTime = game.getUniverse().getTime();
	return m_lastElapsedTime - oldTime;
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
