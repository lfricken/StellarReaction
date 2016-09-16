#pragma once

#include "stdafx.hpp"
#include "Sound.hpp"
#include "NonCopyable.hpp"

namespace leon
{
	class Sound;
}




/// \brief Plays all sounds in game.
///
/// We need a sound manager because there are a max number of sounds. So we want to prevent going over a certain number.
/// Otherwise the program will crash.
class SoundManager : NonCopyable
{
public:
	SoundManager();
	virtual ~SoundManager();

	///Play a sound, and return a handle to it so we can manipulate the sound later.
	int playSound(const std::string& rSoundName, int volume = 100, float minDist = 15, float dropOff = 1, const b2Vec2& rPos = b2Vec2(10,0), bool relative = true, bool looping = false);
	///Get a sound.
	sf::Sound& get(int noiseIndex);
	
protected:
private:
	///Stored in SoundManger. Each Sound only has a certain number of possible noises
	struct Noise
	{
		Noise()
		{
			m_name = "";
			m_startTime = -100;
		}
		std::string m_name;///what sound we are playing now
		sf::Sound m_sound;///here is the sound, get the status
		float m_startTime;///from game time
	};

	/// minimum delay between playing sounds.
	static const float m_minDelay;
	static const std::string m_directory;	
	/// Max sounds that can play (don't mess with this)
	static const int m_numNoises = 256;//citation: http://www.sfml-dev.org/tutorials/2.0/audio-sounds.php
	/// Actual sounds that are playing or have played.
	Noise m_noises[m_numNoises];
	/// Actual sounds
	std::map<std::string, sf::SoundBuffer> m_buffers;


};
