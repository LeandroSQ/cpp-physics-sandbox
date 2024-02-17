#pragma once

#include "../precomp.hpp"
#include "../utils.hpp"
#include "raylib.h"
#include "utils.h"

namespace SoundFX {
	inline raylib::Sound shoot;
	inline raylib::Sound thrust;
	inline raylib::Sound explosion;
	inline raylib::Sound hit;
	inline raylib::Sound spawn;

	static void setup() {
		TraceLog(LOG_INFO, "SoundFX::setup()");

		shoot.Load("assets/audio/shoot.wav");
		if (!shoot.IsReady()) TraceLog(LOG_ERROR, "Failed to load shoot sound");

        thrust.Load("assets/audio/thrust.wav");
		if (!thrust.IsReady()) TraceLog(LOG_ERROR, "Failed to load thrust sound");

		explosion.Load("assets/audio/explosion.wav");
		if (!explosion.IsReady()) TraceLog(LOG_ERROR, "Failed to load explosion sound");

		hit.Load("assets/audio/hit.wav");
		if (!hit.IsReady()) TraceLog(LOG_ERROR, "Failed to load hit sound");

        spawn.Load("assets/audio/spawn.wav");
        if (!spawn.IsReady()) TraceLog(LOG_ERROR, "Failed to load spawn sound");
	}

	static void release() {
		TraceLog(LOG_INFO, "SoundFX::release()");

        shoot.Unload();
        explosion.Unload();
        hit.Unload();
        spawn.Unload();
	}

	static void play(raylib::Sound &sound, float volume = -1.0f, float pitch = -1.0f, float pan = -1.0f) {
		if (!sound.IsReady()) {
		    TraceLog(LOG_ERROR, "SoundFX::play() - Sound is not ready");
		    return;
		}

		if (volume <= 0.0f) volume = 0.5f;
		if (pitch <= 0.0f) pitch = getRandomValue(0.65f, 1.35f);
		if (pan <= 0.0f) pan = getRandomValue(0.25f, 0.75f);

		sound.SetVolume(volume);
		sound.SetPitch(pitch);
		sound.SetPan(pan);
		sound.Play();
	}
};