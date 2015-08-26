#include "SoundManager.hpp"
#include "globals.hpp"

using namespace std;


const float SoundManager::m_minDelay = 0.01f;
const std::string SoundManager::m_directory = "audio/";
SoundManager::SoundManager()
{

}
SoundManager::~SoundManager()
{

}
/// <summary>
/// Use this to play sounds in game.
/// </summary>
void SoundManager::playSound(const SoundData& rSound)
{
	playSound(rSound.name, rSound.vol, rSound.minDist, rSound.dropOff, rSound.pos, rSound.relative);
}
/// <summary>
/// Plays the sound.
/// </summary>
/// <param name="rSoundName">Name and path of the sound (from audio directory)</param>
/// <param name="volume">0-100</param>
/// <param name="minDist">The volume under which it will be indicated volume</param>
/// <param name="dropOff">How quickly the sound drops off, larger means more rapid drop off</param>
/// <param name="rPos">origin</param>
/// <param name="relative">If false, it will be heard globally at it's volume</param>
void SoundManager::playSound(const std::string& rSoundName, int volume, float minDist, float dropOff, const b2Vec2& rPos, bool relative)
{

	if(rSoundName == "")
		return;

	const float time = game.getTime();

	/**See if the sound is already playing**/
	for(int i=0; i<m_numNoises; ++i)
		if(m_noises[i].m_name == rSoundName && (time-m_noises[i].m_startTime) < m_minDelay)
			return;

	/**It's not playing, so see if it is loaded**/
	auto itBuffer = m_buffers.find(rSoundName);
	if(itBuffer == m_buffers.end())//try loading it since it's not
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
			m_noises[i].m_sound.setPosition(rPos.x, 0.f, 0.f);//TODO rPos.y
			m_noises[i].m_sound.play();
			sf::Vector3f d = sf::Listener::getDirection();
			sf::Vector3f p = sf::Listener::getPosition();
			return;
		}
	cout << "\nSounds are full, trying [" << rSoundName << "] " << FILELINE;
}
