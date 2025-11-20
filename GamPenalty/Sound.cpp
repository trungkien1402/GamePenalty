#include "sound.h"
#include "raylib.h"

// Biến tĩnh (static) để lưu trạng thái nhạc nền trước khi tạm dừng bởi SFX
static bool wasMusicPlayingBeforeSFX = false;

// [TRIỂN KHAI CONSTRUCTOR] Khắc phục lỗi VCR001 và C26495
SoundManager::SoundManager() {
    music = { 0 };
    musicLoaded = false;
    goalSFX = { 0 };
    missSFX = { 0 };
}

// === HÀM TẢI NHẠC NỀN ===

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

// Implement hàm LoadSFX
void SoundManager::LoadSFX(const char* goalPath, const char* missPath) {
    goalSFX = LoadSound(goalPath);
    missSFX = LoadSound(missPath);
}

// [HÀM QUẢN LÝ SFX] Kiểm tra xem SFX đã chơi xong chưa
bool SoundManager::IsSFXPlaying() {
    return IsSoundPlaying(goalSFX) || IsSoundPlaying(missSFX);
}

// === HÀM CẬP NHẬT VÀ XỬ LÝ NHẠC NỀN ===

void SoundManager::UpdateMusic() {
    // 1. Cập nhật stream nhạc nếu đang chơi
    if (musicLoaded && IsMusicStreamPlaying(music)) {
        UpdateMusicStream(music);
    }

    // 2. [LOGIC RESUME]
    // Nếu nhạc đã bị tạm dừng do SFX VÀ SFX đã chơi xong THÌ: Bật nhạc lại
    if (musicLoaded && wasMusicPlayingBeforeSFX && !IsMusicStreamPlaying(music) && !IsSFXPlaying()) {
        ResumeMusicStream(music);
        wasMusicPlayingBeforeSFX = false; // Reset cờ
    }
}

// === CÁC HÀM PHÁT ===

// [FIX LỖI C2039/C2065] Hàm PlayMusic
void SoundManager::PlayMusic() {
    if (musicLoaded) {
        PlayMusicStream(music);
    }
}

void SoundManager::ToggleMusic() {
    if (!musicLoaded) return;
    if (IsMusicStreamPlaying(music)) PauseMusicStream(music);
    else ResumeMusicStream(music);
}

bool SoundManager::IsMusicPlaying() {
    if (!musicLoaded) return false;
    return IsMusicStreamPlaying(music);
}

// Implement hàm PlaySFX (có xử lý Pause/Resume nhạc nền)
void SoundManager::PlayGoalSFX() {
    wasMusicPlayingBeforeSFX = IsMusicStreamPlaying(music);
    if (wasMusicPlayingBeforeSFX) PauseMusicStream(music);
    PlaySound(goalSFX);
}

void SoundManager::PlayMissSFX() {
    wasMusicPlayingBeforeSFX = IsMusicStreamPlaying(music);
    if (wasMusicPlayingBeforeSFX) PauseMusicStream(music);
    PlaySound(missSFX);
<<<<<<< HEAD
}
=======
}
>>>>>>> 9b706097fc97aff184be64f9d710b8c7030e988f
