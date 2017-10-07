#pragma once

class Clock
{
public:
	Clock();
	~Clock();

	virtual float getTime() const = 0;
};

