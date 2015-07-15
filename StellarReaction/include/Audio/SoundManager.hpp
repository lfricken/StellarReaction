#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include "stdafx.hpp"
#include "SoundData.hpp"

struct Noise
{
	Noise()
	{
		m_name = "";
		m_startTime = -100;
	}
	std::string m_name;//what sound we are playing now
	sf::Sound m_sound;//here is the sound, get the status
	float m_startTime;//from game time
};

class SoundManager
{
public:
	SoundManager();
	virtual ~SoundManager();

	void playSound(const SoundData& rSound);
	void playSound(const std::string& rSoundName, int volume = leon::volume, float minDist = leon::dist, float dropOff = leon::drop, const b2Vec2& rPos = leon::dPos, bool relative = true);


protected:
private:
	static const float m_minDelay;
	static const std::string m_directory;
	static const int m_numNoises = 128;
	Noise m_noises[m_numNoises];
	std::map<std::string, sf::SoundBuffer> m_buffers;
};

#endif // SOUNDMANAGER_HPP
