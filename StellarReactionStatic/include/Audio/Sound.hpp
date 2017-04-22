#pragma once

#include "stdafx.hpp"
#include "JSON.hpp"

namespace leon
{	
	///A sound acts as a wrapper around a sound handle for easier playing of sounds.
	class Sound
	{
	public:
		Sound(const String& rSoundName = "", const Vec2& pos = Vec2(0,0), int volume = 50, float minDist = 15, float dropOff = 1, bool relative = true, bool looping = false, bool acquiresLock = false);
		virtual ~Sound();

		///Play this sound and allow an old sound to keep playing.
		void play();
		///Play this sound and allow an old sound to keep playing.
		void play(const Vec2& rPos);
		///Play sound from beginning.
		void restart();
		///Continue playing sound, no effect if already playing.
		void resume();
		///Pause this sound.
		void pause();

		///Update position of this sound.
		void setPos(const Vec2& rPos);


		String name;
		Vec2 pos;
		int volume;

		float minDist;
		float dropOff;

		bool relative;
		bool shouldLoop;

		///Fill this object with data from a json file.
		virtual void loadJson(const Json::Value& root)
		{
			GETJSON(name);
			GETJSON(pos);
			GETJSON(volume);

			GETJSON(minDist);
			GETJSON(dropOff);

			GETJSON(relative);
			GETJSON(shouldLoop);
		}

	private:
		friend class SoundManager;


		bool haveHandle() const;
		int m_soundHandle;
		bool m_acquiresLock;
	};
}
