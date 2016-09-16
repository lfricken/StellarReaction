#include "Sound.hpp"
#include "Globals.hpp"
#include "SoundManager.hpp"

using namespace leon;

Sound::Sound(const std::string& rSoundName, int volume, float minDist, float dropOff, bool relative, bool looping)
{
	this->name = rSoundName;
	this->volume = volume;

	this->minDist = minDist;
	this->dropOff = dropOff;

	this->relative = relative;
	this->shouldLoop = looping;

	m_soundHandle = -1;
}
Sound::~Sound()
{


}
void Sound::play(const Vec2& rPos)
{
	game.getSound().playSound(name, volume, minDist, dropOff, rPos, relative, shouldLoop);//
}
void Sound::setPos(const Vec2& rPos)
{
	game.getSound().get(m_soundHandle).setPosition(sf::Vector3f(rPos.x, rPos.y, 0));
}
void Sound::restart(const Vec2& rPos)//restarts from beginning no matter what
{
	game.getSound().get(m_soundHandle).stop();//reset position
	game.getSound().get(m_soundHandle).play();//play from beginning
	game.getSound().get(m_soundHandle).setPosition(sf::Vector3f(rPos.x, rPos.y, 0));
}
void Sound::resume(const Vec2& rPos)
{
	if(game.getSound().get(m_soundHandle).getStatus() != sf::SoundSource::Status::Playing)
	{
		game.getSound().get(m_soundHandle).play();
		game.getSound().get(m_soundHandle).setPosition(sf::Vector3f(rPos.x, rPos.y, 0));
	}
}
void Sound::pause()//pause the sound
{
	game.getSound().get(m_soundHandle).pause();
}
