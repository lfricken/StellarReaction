#ifndef SOUNDDATA_HPP
#define SOUNDDATA_HPP

#include "JSON.hpp"

//Used to pass to SoundManager to play a sound with these settings.
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

	//Fill this object with data from a json file.
	virtual void loadJson(const Json::Value& root)
	{
		GETJSON(name);
		GETJSON(volume);

		GETJSON(dropOff);
		GETJSON(minDist);

		GETJSON(relative);
		GETJSON(shouldLoop);
	}
};

#endif // SOUNDDATA_HPP
