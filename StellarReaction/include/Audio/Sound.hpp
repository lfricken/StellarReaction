#pragma once

#include "stdafx.hpp"

struct SoundData;

namespace leon
{	
	/// <summary>
	/// A class to make an instance of for a class. When you want to play a sound, use this.
	/// TODO
	/// </summary>
	class Sound
	{
	public:
		Sound(const SoundData& rData);
		virtual ~Sound();

		void setPos(const b2Vec2& rPos);
		void toggleLooping(bool shouldLoop);
		void toggleLooping();

		void restart();//restarts from beginning no matter what
		void resume();//continue playing, may have been restarted
		void pause();//pause the sound

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
