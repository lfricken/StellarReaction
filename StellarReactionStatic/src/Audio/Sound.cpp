#include "Sound.hpp"
#include "SoundData.hpp"
#include "Globals.hpp"
#include "SoundManager.hpp"

using namespace leon;

Sound::Sound(const SoundData& rData)
{
	m_soundName = rData.name;
	m_volume = rData.volume;
	m_shouldLoop = rData.shouldLoop;
	m_dropOff = rData.dropOff;
	m_minDist = rData.minDist;
	m_relative = rData.relative;
}
Sound::~Sound()
{

}
void Sound::setPos(const b2Vec2& rPos)
{
	m_pos = rPos;
}
void Sound::toggleLooping(bool shouldLoop)
{
	m_shouldLoop = shouldLoop;
}
void Sound::toggleLooping()
{
	this->toggleLooping(!m_shouldLoop);
}
void Sound::restart()//restarts from beginning no matter what
{
	//game.get
}
void Sound::resume()//continue playing, may have been restarted
{

}
void Sound::pause()//pause the sound
{

}
