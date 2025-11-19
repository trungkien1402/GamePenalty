#pragma once
#include "raylib.h"

class SoundManager {
private:
    Music music;
    bool musicLoaded = false;

public:
    void LoadMusic(const char* path);
    void PlayMusic();
    void UpdateMusic();
    void ToggleMusic();
    bool IsMusicPlaying();
};
