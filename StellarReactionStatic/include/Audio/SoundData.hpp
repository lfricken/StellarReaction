#ifndef SOUNDDATA_HPP
#define SOUNDDATA_HPP

#include "JSON.hpp"

/// <summary>
/// Used to pass to SoundManager to play a sound with these settings.
/// </summary>
struct SoundData
{
	SoundData()
	{
		name = "";
		volume = 100;
		minDist = 5;
		dropOff = 30;
		pos = b2Vec2(1,0);
		relative = false;
		shouldLoop = false;
	}

	std::string name;
	int volume;

	float minDist;
	float dropOff;

	bool relative;
	bool shouldLoop;

	b2Vec2 pos;

	virtual void loadJson(const Json::Value& root)
	{
		GETJSON(name);
		GETJSON(volume);
		GETJSON(dropOff);
		GETJSON(minDist);
		GETJSON(relative);
	}
};

#endif // SOUNDDATA_HPP
