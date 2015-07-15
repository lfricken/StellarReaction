#ifndef SOUNDDATA_HPP
#define SOUNDDATA_HPP

namespace leon
{
	const int volume = 100;
	const float dist = 5;
	const int drop = 50;
	const  b2Vec2 dPos(1,0);
}

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
	}

	std::string name;
	int vol;
	float minDist;
	float dropOff;
	b2Vec2 pos;
	bool relative;
};

#endif // SOUNDDATA_HPP
