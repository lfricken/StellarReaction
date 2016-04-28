#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include "stdafx.hpp"
#include "SoundData.hpp"
#include "Sound.hpp"
#include "NonCopyable.hpp"

namespace leon
{
	class Sound;
}

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


/// \brief Plays all sounds in game.
///
/// We need a sound manager because there are a max number of sounds. So we want to prevent going over a certain number.
/// Otherwise the program will crash.
class SoundManager : NonCopyable
{
public:
	SoundManager();
	virtual ~SoundManager();

	///Play a particular sound once.
	void playSound(const SoundData& rSound);
	///Play a sound, and return a handle to it so we can manipulate the sound later.
	int playSound(const std::string& rSoundName, int volume = 100, float minDist = 5, float dropOff = 20, const b2Vec2& rPos = b2Vec2(1,0), bool relative = true, bool looping = false);
	///Stop a sound, with a handle.
	int stopSound(int noiseIndex);

	friend class leon::Sound;
	
protected:
private:
	/// minimum delay between playing sounds.
	static const float m_minDelay;
	static const std::string m_directory;	
	/// Max sounds that can play (don't mess with this)
	static const int m_numNoises = 255;//citation: http://www.sfml-dev.org/tutorials/2.0/audio-sounds.php
	/// Actual sounds that are playing or have played.
	Noise m_noises[m_numNoises];
	/// \brief map of (string soundname, sound thing which stores the actual data)
	///
	/// We load these into sf::Sounds to play them
	std::map<std::string, sf::SoundBuffer> m_buffers;
};

#endif // SOUNDMANAGER_HPP
