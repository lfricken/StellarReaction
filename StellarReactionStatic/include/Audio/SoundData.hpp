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
};

#endif // SOUNDDATA_HPP
