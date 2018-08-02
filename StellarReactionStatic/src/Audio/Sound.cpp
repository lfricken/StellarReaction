#include "Sound.hpp"
#include "Globals.hpp"
#include "SoundManager.hpp"

using namespace leon;

Sound::Sound(const String& rSoundName, const Vec2& pos, int volume, float minDist, float dropOff, bool relative, bool looping, bool acquiresLock)
{
	this->name = rSoundName;
	this->pos = pos;
	this->volume = volume;

	this->minDist = minDist;
	this->dropOff = dropOff;

	this->relative = relative;
	this->shouldLoop = looping;

	this->m_acquiresLock = acquiresLock;

	m_soundHandle = -1;
}
Sound::~Sound()
{

}
bool Sound::haveHandle() const
{
	return (m_soundHandle != -1);
}
void Sound::play()
{
	m_soundHandle = getGame()->getSound().playSound(*this);
}
void Sound::play(const Vec2& rPos)
{
	pos = rPos;
	m_soundHandle = getGame()->getSound().playSound(*this);
}
void Sound::restart()//restarts from beginning no matter what
{
	if(haveHandle())
	{
		auto& sound = getGame()->getSound().get(m_soundHandle);
		sound.stop();//reset playing position
		sound.play();//play from beginning
	}
	else
		m_soundHandle = getGame()->getSound().playSound(*this);
}
void Sound::resume()
{
	if(haveHandle())
	{
		auto& sound = getGame()->getSound().get(m_soundHandle);

		if(sound.getStatus() != sf::SoundSource::Status::Playing)
			sound.play();
	}
	else
		play();
}

void Sound::pause()//pause the sound
{
	if(haveHandle())
		getGame()->getSound().get(m_soundHandle).pause();
}
void Sound::setPos(const Vec2& rPos)
{
	pos = rPos;

	if(haveHandle())
		getGame()->getSound().get(m_soundHandle).setPosition(sf::Vector3f(pos.x, pos.y, 0));
}