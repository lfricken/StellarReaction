#pragma once

#include "Clock.hpp"

/// <summary>
/// Universal timer for in game and GUI. GUI needs to pass game object as clock.
/// Defaults to Universe clock.
/// </summary>
class Timer
{
public:
	Timer();
	Timer(float time);
	Timer(const Clock& clockRef);
	virtual ~Timer();

	/// <summary>
	/// Return a percentage on how much time has elapsed.
	/// Return 1.0f when isTimeUp returns true.
	/// </summary>
	float getTimePercentageElapsed() const;

	/// <summary>
	/// Time elapsed as seconds since the universe was initialized.
	/// </summary>
	float getTime() const;
	/// <summary>
	/// Get time elapsed since we last called this function on this instance, or our since our initialization.
	/// </summary>
	float getTimeElapsed();

	/// <summary>
	/// Has the timer expired?
	/// </summary>
	bool isTimeUp() const;
	/// <summary>
	/// Sets the countdown time. Defaults to 1 second.
	/// </summary>
	void setCountDown(float countDown = 1.f);
	/// <summary>
	/// Sets the REMAINING countdown time to the countdown time.
	/// </summary>
	void restartCountDown();
protected:
private:
	/// <summary>
	/// Reference to the clock that dictates time passage.
	/// </summary>
	const Clock* clock;

	/// <summary>
	/// The time that getTimeElapsed was called last.
	/// </summary>
	float m_lastTimeElapsed;

	/// <summary>
	/// How long the timer should go before isTimeUp will return true.
	/// Does not change, is used to restart countdown.
	/// </summary>
	float m_countDownTime;
	/// <summary>
	/// Time when the timer isTimeUp will return true.
	/// </summary>
	float m_timeUpTime;
};
