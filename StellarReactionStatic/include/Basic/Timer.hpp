#pragma once

/// \brief Timer for in game items, NOT FOR GUI.
///
/// The reason is that pausing the game pauses these timers.
/// Use this for weapons, animations.
/// If the "Pause" function should effect your item, use this class.
class Timer
{
public:
	Timer();
	Timer(float time);
	virtual ~Timer();

	///Time elapsed as seconds since the universe was initialized.
	float getTime() const;
	///Get time elapsed since we last called this function on this instance, or our since our initialization.
	float getTimeElapsed();

	///Has the timer expired?
	bool isTimeUp() const;
	///Sets the countdown time. Defaults to 1 second.
	void setCountDown(float countDown = 1.f);
	///Sets the REMAINING countdown time to the countdown time.
	void restartCountDown();
protected:
private:
	float m_lastElapsedTime;

	float m_countDownTime;
	float m_timeUpTime;
};
