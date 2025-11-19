#include "sound.h"
#include "raylib.h"

void SoundManager::LoadMusic(const char* path) {
    music = LoadMusicStream(path);
    if (music.ctxData != NULL) {
        musicLoaded = true;
        music.looping = true;
    }
    else {
        TraceLog(LOG_ERROR, "FAILED TO LOAD MUSIC: %s", path);
    }
}

void SoundManager::PlayMusic() { if (musicLoaded) PlayMusicStream(music); }
void SoundManager::UpdateMusic() { if (musicLoaded) UpdateMusicStream(music); }
void SoundManager::ToggleMusic() {
    if (!musicLoaded) return;
    if (IsMusicStreamPlaying(music)) PauseMusicStream(music);
    else ResumeMusicStream(music);
}
bool SoundManager::IsMusicPlaying() {
    if (!musicLoaded) return false;
    return IsMusicStreamPlaying(music);
}
