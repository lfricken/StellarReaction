#ifndef TIMER_H
#define TIMER_H

/// <summary>
/// Timer for in game items, NOT FOR GUI.
/// The reason is that pausing the game pauses these timers.
/// Use this for weapons, animations.
/// If the "Pause" function should effect your item use this
/// </summary>
class Timer
{
public:
	Timer();
	Timer(float time);
	virtual ~Timer();

	float getTime() const;//time elapsed as seconds since the universe was initialized
	float getTimeElapsed();//get time elapsed since we last called this function, or our initialization

	bool isTimeUp() const;//asks whether the timer has counted down enough
	void setCountDown(float countDown);//sets the countdownTime, default 1.0
	void restartCountDown();//starts, the current countdown time from whatever it is supposed to be
protected:
private:
	float m_lastElapsedTime;

	float m_countDownTime;
	mutable float m_lastCountDownTime;
	mutable float m_countDownTimeRemain;
};

#endif // TIMER_H
