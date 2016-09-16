#pragma once

#include "stdafx.hpp"
#include "JSON.hpp"

namespace leon
{	
	///A sound acts as a wrapper around a sound handle for easier playing of sounds.
	class Sound
	{
	public:
		Sound(const std::string& rSoundName = "", int volume = 100, float minDist = 15, float dropOff = 1, bool relative = true, bool looping = false);
		virtual ~Sound();

		///Play this sound and allow an old sound to keep playing.
		void play(const Vec2& rPos);
		///Play sound from beginning.
		void restart(const Vec2& rPos);
		///Continue playing sound, no effect if already playing.
		void resume(const Vec2& rPos);
		///Pause this sound.
		void pause();


		///Update position of this sound.
		void setPos(const Vec2& rPos);

		std::string name;
		int volume;

		float minDist;
		float dropOff;

		bool relative;
		bool shouldLoop;

		///Fill this object with data from a json file.
		virtual void loadJson(const Json::Value& root)
		{
			GETJSON(name);
			GETJSON(volume);

			GETJSON(minDist);
			GETJSON(dropOff);

			GETJSON(relative);
			GETJSON(shouldLoop);
		}

	private:
		int m_soundHandle;
	};
}
