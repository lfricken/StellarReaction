#include "SoundManager.hpp"
#include "globals.hpp"

using namespace std;

const float SoundManager::m_minDelay = 0.01;
const std::string SoundManager::m_directory = "audio/";
SoundManager::SoundManager()
{

}
SoundManager::~SoundManager()
{

}
void SoundManager::playSound(const SoundData& rSound)
{
	playSound(rSound.name, rSound.vol, rSound.minDist, rSound.dropOff, rSound.pos, rSound.relative);
}
void SoundManager::playSound(const std::string& rSoundName, int volume, float minDist, float dropOff, const b2Vec2& rPos, bool relative)
{
	if(rSoundName == "")
		return;

	const float time = game.getTime();

	/**See if the sound is already playing**/
	for(int i=0; i<m_numNoises; ++i)
		if(m_noises[i].m_name == rSoundName && time-m_noises[i].m_startTime < m_minDelay)
			return;

	/**It's not playing, so see if it is loaded**/
	auto itBuffer = m_buffers.find(rSoundName);
	if(itBuffer == m_buffers.end())//try loading it
	{
		if(!m_buffers[rSoundName].loadFromFile(m_directory+rSoundName))
		{
			cout << "\nError loading sound [" << m_directory+rSoundName << "] " << FILELINE;
			///ERROR LOG
			return;
		}
		itBuffer = m_buffers.find(rSoundName);
	}


	/**It's been loaded, find a noise to play it in**/
	for(int i=0; i<m_numNoises; ++i)
		if(m_noises[i].m_sound.getStatus() == sf::Sound::Stopped)
		{
			m_noises[i].m_name = rSoundName;
			m_noises[i].m_startTime = time;

			m_noises[i].m_sound.setBuffer(itBuffer->second);
			m_noises[i].m_sound.setMinDistance(minDist);
			m_noises[i].m_sound.setRelativeToListener(relative);
			m_noises[i].m_sound.setAttenuation(dropOff);
			m_noises[i].m_sound.setVolume(volume);
			m_noises[i].m_sound.setPosition(rPos.x, rPos.y, 0.0f);
			m_noises[i].m_sound.play();
			return;
		}
	cout << "\nSounds full trying [" << rSoundName << "] " << FILELINE;
}
