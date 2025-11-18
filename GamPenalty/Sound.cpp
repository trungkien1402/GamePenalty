#include "sound.h"
#include "raylib.h"

void SoundManager::LoadMusic(const char* path)
{
    music = LoadMusicStream(path);
    if (music.ctxData != NULL) {
        musicLoaded = true;
        music.looping = true;
    }
}

void SoundManager::PlayMusic()
{
    if (musicLoaded) {
        PlayMusicStream(music);
    }
}

void SoundManager::UpdateMusic()
{
    if (musicLoaded) {
        UpdateMusicStream(music);
    }
}

void SoundManager::ToggleMusic()
{
    if (!musicLoaded) return;

    // Đang chơi -> pause
    if (IsMusicPlaying()) {
        PauseMusicStream(music);
    }
    // Đang tắt -> resume
    else {
        ResumeMusicStream(music);
    }
}

bool SoundManager::IsMusicPlaying()
{
    if (!musicLoaded) return false;

    return IsAudioStreamPlaying(music.stream);

}
