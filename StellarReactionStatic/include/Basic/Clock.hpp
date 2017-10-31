#pragma once

/// <summary>
/// Can tell time, used for Timers.
/// </summary>
class Clock
{
public:
	Clock();
	~Clock();

	virtual float getTime() const = 0;
};

