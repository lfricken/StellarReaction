#ifndef SOUNDDATA_HPP
#define SOUNDDATA_HPP

namespace leon
{
	const int volume = 100;
	const float dist = 5;
	const int drop = 30;
	const b2Vec2 dPos(1,0);
}

/// <summary>
/// Used to pass to SoundManager to play a sound with these settings.
/// </summary>
struct SoundData
{
	SoundData()
	{
		name = "";
		vol = leon::volume;
		minDist = leon::dist;
		dropOff = leon::drop;
		pos = leon::dPos;
		relative = false;
		shouldLoop = false;
	}

	std::string name;
	int vol;

	float minDist;
	float dropOff;

	bool relative;
	bool shouldLoop;

	b2Vec2 pos;

	virtual void loadJson(const Json::Value& root)
	{
		if(!root["name"].isNull())
			name = root["name"].asString();
		if(!root["vol"].isNull())
			vol = root["vol"].asInt();
		if(!root["dropOff"].isNull())
			dropOff = root["dropOff"].asFloat();
		if(!root["minDist"].isNull())
			minDist = root["minDist"].asFloat();
		if(!root["relative"].isNull())
			relative = root["relative"].asBool();
	}
};

#endif // SOUNDDATA_HPP
