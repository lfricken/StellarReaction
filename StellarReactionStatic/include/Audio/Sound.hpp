#pragma once

#include "stdafx.hpp"

struct SoundData;

namespace leon
{	
	///A sound acts as a wrapper around a sound handle for easier playing of sounds.
	class Sound
	{
	public:
		Sound(const SoundData& rData);
		virtual ~Sound();

		///Sets the position of this sound, so the sound emanates from this position.
		void setPos(const b2Vec2& rPos);
		///Determines whether this sound will loop or not.
		void toggleLooping(bool shouldLoop);
		///Switches whether this sound will loop or not.
		void toggleLooping();
		///Play sound from beginning.
		void restart();
		///Continue playing sound, may cause restart.
		void resume();
		///Pause this sound.
		void pause();

	protected:

	private:
		std::string m_soundName;
		int m_volume;

		float m_minDist;
		float m_dropOff;

		bool m_relative;
		bool m_shouldLoop;

		b2Vec2 m_pos;
	};
}
