#include "SoundManager.hpp"
#include "Sound.hpp"
#include "Globals.hpp"
#include "Debugging.hpp"


const String SoundManager::m_directory = "audio/";
SoundManager::SoundManager()
{
	//m_defaultNoise.sound.setBuffer(itBuffer->second);
	//m_defaultNoise.sound.setMinDistance(minDist);
	//m_defaultNoise.sound.setRelativeToListener(!relative);//reverse because relative SHOULD mean not absolute
	//m_defaultNoise.sound.setAttenuation(dropOff);
	//m_defaultNoise.sound.setVolume((float)volume);
	//m_defaultNoise.sound.setPosition(rPos.x, rPos.y, 0.f);//TODO rPos.y
	//m_defaultNoise.sound.setLoop(looping);

	//m_buffers[
}
SoundManager::~SoundManager()
{

}
sf::SoundBuffer* SoundManager::loadSoundBuffer(const String& rSoundName)
{
	/**Make sure the sound is loaded.**/
	auto itBuffer = m_buffers.find(rSoundName);
	if(itBuffer == m_buffers.end()) //try loading it since it's not
	{
		if(!m_buffers[rSoundName].loadFromFile(contentDir() + m_directory + rSoundName))
		{
			Print << "\nError loading sound [" << contentDir() + m_directory + rSoundName << "]";
			return nullptr;
		}
		itBuffer = m_buffers.find(rSoundName);
	}
	return &(itBuffer->second);
}
int SoundManager::playSound(const leon::Sound& sound)
{
	return playSound(sound.name, sound.pos, sound.volume, sound.minDist, sound.dropOff, sound.relative, sound.shouldLoop, sound.m_acquiresLock);
}
int SoundManager::playSound(const String& rSoundName, const Vec2& rPos, int volume, float minDist, float dropOff, bool relative, bool looping, bool aquireLock)
{
	const float time = getGame()->getTime();
	auto i = getFreeNoise();
	auto soundBuffer = loadSoundBuffer(rSoundName);

	if(soundBuffer == nullptr || i == -1)
		return -1;

	m_noises[i].locked = aquireLock;
	m_noises[i].sound.setBuffer(*soundBuffer);
	m_noises[i].sound.setMinDistance(minDist);
	m_noises[i].sound.setRelativeToListener(!relative);//reverse because relative SHOULD mean not absolute
	m_noises[i].sound.setAttenuation(dropOff);
	m_noises[i].sound.setVolume((float)volume);
	m_noises[i].sound.setPosition(rPos.x, rPos.y, 0.f);
	m_noises[i].sound.setLoop(looping);
	m_noises[i].sound.play();

	if(aquireLock)
		return i;
	else
		return -1;
}
sf::Sound& SoundManager::get(int noiseIndex)
{
	if(noiseIndex < m_numNoises && noiseIndex >= 0)
		return m_noises[noiseIndex].sound;
	else
		return m_defaultNoise.sound;
}
int SoundManager::getFreeNoise() const
{
	for(int i = 0; i < m_numNoises; ++i)
		if(m_noises[i].sound.getStatus() != sf::Sound::Playing && m_noises[i].locked == false)//sound is done and not locked
			return i;

	Print << "\nNo Noises left!" << FILELINE;
	return -1;
}