#pragma once
#include "raylib.h"

class SoundManager {
private:
    Music music;
    bool musicLoaded = false;

    // Khởi tạo các biến Sound ngay trong khai báo (Để khắc phục C26495)
    Sound goalSFX = { 0 };
    Sound missSFX = { 0 };

public:
    // [KHAI BÁO CONSTRUCTOR] Khắc phục lỗi thiếu hàm tạo
    SoundManager();

    void LoadMusic(const char* path);

    // Hàm UpdateMusic cần được đặt ở đây
    void UpdateMusic();

    void PlayMusic(); // Khắc phục lỗi "PlayMusic không tồn tại"
    void ToggleMusic();
    bool IsMusicPlaying();

    void LoadSFX(const char* goalPath, const char* missPath);
    void PlayGoalSFX();
    void PlayMissSFX();
    bool IsSFXPlaying();
};
