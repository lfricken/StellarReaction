#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include "stdafx.hpp"
#include "SoundData.hpp"
#include "Sound.hpp"

namespace leon
{
	class Sound;
}
/// <summary>
/// Stored in SoundManger. Each Sound only has a certain number of 
/// </summary>
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

/// <summary>
/// Plays all sounds in game
/// </summary>
class SoundManager
{
public:
	SoundManager();
	virtual ~SoundManager();

	void playSound(const SoundData& rSound);
	void playSound(const std::string& rSoundName, int volume = leon::volume, float minDist = leon::dist, float dropOff = leon::drop, const b2Vec2& rPos = leon::dPos, bool relative = true);

	friend class leon::Sound;
	
protected:
private:
	/// <summary>
	/// minimum delay between playing sounds.
	/// </summary>
	static const float m_minDelay;
	static const std::string m_directory;	
	/// <summary>
	/// Max sounds that can play (don't mess with this)
	/// </summary>
	static const int m_numNoises = 255;//citation: http://www.sfml-dev.org/tutorials/2.0/audio-sounds.php
	/// <summary>
	/// Actual sounds that are playing or have played.
	/// </summary>
	Noise m_noises[m_numNoises];
	/// <summary>
	/// map of (string soundname, sound thing which stores the actual data)
	/// We load these into sf::Sounds to play them
	/// </summary>
	std::map<std::string, sf::SoundBuffer> m_buffers;
};

#endif // SOUNDMANAGER_HPP
