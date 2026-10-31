#pragma once
#include "Includes.h"

class SoundManager;

class SoundManager {
public:
	Music gameMusic;
	Music cave;
	Sound earthquake;
	SoundManager() {
		
		//load music and sound on constructor
		gameMusic = LoadMusicStream("audio/arins_adventure.mp3");
		cave = LoadMusicStream("audio/cave.mp3");
		earthquake = LoadSound("audio/earth.mp3");
	}


	//deconstuctor unload all music game sounds
	~SoundManager()
	{
		StopMusicStream(gameMusic);
		StopMusicStream(cave);
		UnloadMusicStream(gameMusic);
		UnloadMusicStream(cave);
		UnloadSound(earthquake);
	}
};