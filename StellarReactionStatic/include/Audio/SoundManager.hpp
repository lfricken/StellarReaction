#pragma once

#include "stdafx.hpp"
#include "NonCopyable.hpp"

namespace leon
{
	class Sound;
}




/// \brief Plays all sounds in getGame()->
///
/// We need a sound manager because there are a max number of sounds. So we want to prevent going over a certain number.
/// Otherwise the program will crash.
class SoundManager : NonCopyable
{
public:
	SoundManager();
	virtual ~SoundManager();

	///Play a sound, and return a handle to it so we can manipulate the sound later.
	int playSound(const leon::Sound& sound);
	///Play a sound, and return a handle to it so we can manipulate the sound later.
	int playSound(const String& rSoundName, const Vec2& rPos = Vec2(10, 0), int volume = 100, float minDist = 15, float dropOff = 1, bool relative = true, bool looping = false, bool aquireLock = false);
	///Get a sound.
	sf::Sound& get(int noiseIndex);
	
protected:
private:
	///Stored in SoundManger. Each Sound only has a certain number of possible noises
	struct Noise
	{
		Noise()
		{
			locked = false;
		}
		sf::Sound sound;///here is the sound, get the status
		/// If locked, a sound is being referenced by a handle and should not be repurposed.
		bool locked;
	};

	///Load a sound into buffer if needed.
	sf::SoundBuffer* loadSoundBuffer(const String& rSoundName);
	///Gets a noise that isn't doing anything right now.
	int getFreeNoise() const;



	/// Audio directory from content folder.
	static const String m_directory;
	/// Max sounds that can play (don't mess with this)
	static const int m_numNoises = 250;//256 citation: http://www.sfml-dev.org/tutorials/2.0/audio-sounds.php
	/// Sprite instances with a max number at once.
	Noise m_noises[m_numNoises];
	/// Textures, unlimited at once.
	std::map<String, sf::SoundBuffer> m_buffers;
	/// Default Noise
	Noise m_defaultNoise;
};
