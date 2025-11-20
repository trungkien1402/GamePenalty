#include "sound.h" // Bao gồm khai báo lớp SoundManager
#include "raylib.h" // Thư viện Raylib cho các hàm xử lý âm thanh

// Biến tĩnh (static) để ghi nhớ trạng thái phát nhạc nền trước khi SFX bắt đầu.
// Biến này có phạm vi nội bộ (chỉ trong file .cpp này).
static bool wasMusicPlayingBeforeSFX = false;

// [TRIỂN KHAI CONSTRUCTOR] Hàm tạo mặc định cho lớp SoundManager
SoundManager::SoundManager() {
    // Khởi tạo tất cả các thuộc tính Music và Sound về giá trị 0
    music = { 0 };
    musicLoaded = false;
    goalSFX = { 0 };
    missSFX = { 0 };
}

// === HÀM TẢI NHẠC NỀN ===

void SoundManager::LoadMusic(const char* path) {
    // Tải nhạc nền dưới dạng stream (phát từng phần) để tiết kiệm bộ nhớ RAM
    music = LoadMusicStream(path);
    if (music.ctxData != NULL) { // Kiểm tra trạng thái tải thành công
        musicLoaded = true;
        music.looping = true; // Thiết lập nhạc nền tự động lặp lại khi kết thúc
    }
    else {
        // Ghi log lỗi nếu không tìm thấy hoặc không tải được file nhạc
        TraceLog(LOG_ERROR, "FAILED TO LOAD MUSIC: %s", path);
    }
}

// Implement hàm LoadSFX
void SoundManager::LoadSFX(const char* goalPath, const char* missPath) {
    // Tải các file âm thanh hiệu ứng (SFX) vào bộ nhớ (RAM)
    goalSFX = LoadSound(goalPath);
    missSFX = LoadSound(missPath);
}

// [HÀM QUẢN LÝ SFX] Kiểm tra trạng thái phát SFX
bool SoundManager::IsSFXPlaying() {
    // Trả về true nếu bất kỳ hiệu ứng âm thanh nào đang được phát
    return IsSoundPlaying(goalSFX) || IsSoundPlaying(missSFX);
}

// === HÀM CẬP NHẬT VÀ XỬ LÝ NHẠC NỀN ===

void SoundManager::UpdateMusic() {
    // 1. Cập nhật stream nhạc
    // Cần gọi hàm này liên tục (mỗi frame) để duy trì việc phát nhạc stream
    if (musicLoaded && IsMusicStreamPlaying(music)) {
        UpdateMusicStream(music);
    }

    // 2. [LOGIC TỰ ĐỘNG PHÁT LẠI]
    // Kiểm tra nếu nhạc bị tạm dừng do SFX và SFX đã chơi xong
    if (musicLoaded && wasMusicPlayingBeforeSFX && !IsMusicStreamPlaying(music) && !IsSFXPlaying()) {
        ResumeMusicStream(music); // Bật nhạc nền trở lại
        wasMusicPlayingBeforeSFX = false; // Đặt lại cờ trạng thái
    }
}

// === CÁC HÀM PHÁT ===

// Hàm PlayMusic
void SoundManager::PlayMusic() {
    if (musicLoaded) {
        PlayMusicStream(music); // Bắt đầu phát nhạc nền từ đầu
    }
}

void SoundManager::ToggleMusic() {
    if (!musicLoaded) return;
    // Tạm dừng hoặc tiếp tục phát nhạc nền
    if (IsMusicStreamPlaying(music)) PauseMusicStream(music);
    else ResumeMusicStream(music);
}

bool SoundManager::IsMusicPlaying() {
    if (!musicLoaded) return false;
    // Trả về trạng thái phát của nhạc nền
    return IsMusicStreamPlaying(music);
}

// Implement hàm PlaySFX (có xử lý tạm dừng nhạc nền)
void SoundManager::PlayGoalSFX() {
    // 1. Lưu lại trạng thái phát nhạc nền
    wasMusicPlayingBeforeSFX = IsMusicStreamPlaying(music);
    // 2. Tạm dừng nhạc nền nếu nó đang chạy (để ưu tiên SFX)
    if (wasMusicPlayingBeforeSFX) PauseMusicStream(music);
    // 3. Phát hiệu ứng âm thanh ghi bàn
    PlaySound(goalSFX);
}

void SoundManager::PlayMissSFX() {
    // 1. Lưu lại trạng thái phát nhạc nền
    wasMusicPlayingBeforeSFX = IsMusicStreamPlaying(music);
    // 2. Tạm dừng nhạc nền nếu nó đang chạy (để ưu tiên SFX)
    if (wasMusicPlayingBeforeSFX) PauseMusicStream(music);
    // 3. Phát hiệu ứng âm thanh sút trượt
    PlaySound(missSFX);
}
