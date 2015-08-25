#include "Timer.hpp"
#include "globals.hpp"

#include "Universe.hpp"

Timer::Timer()
{
	m_lastElapsedTime = game.getUniverse().getTime();
	m_lastCountDownTime = game.getUniverse().getTime();
	m_countDownTime = 1.0f;
	m_countDownTimeRemain = 0;
}
Timer::~Timer()
{

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
	m_countDownTimeRemain -= (game.getUniverse().getTime() - m_lastCountDownTime);
	m_lastCountDownTime = game.getUniverse().getTime();
	return (m_countDownTimeRemain <= 0);
}
void Timer::setCountDown(float countDown)
{
	m_countDownTime = countDown;
}
void Timer::restartCountDown()
{
	m_lastCountDownTime = getTime();
	m_countDownTimeRemain = m_countDownTime;
}
