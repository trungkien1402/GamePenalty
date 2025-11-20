#pragma once
// Đảm bảo file header này chỉ được include một lần (giúp biên dịch nhanh hơn và tránh lỗi).

#include "raylib.h"
// Cần include thư viện Raylib vì lớp này sử dụng các kiểu dữ liệu âm thanh cốt lõi của Raylib: Music và Sound.

class SoundManager {
private:
    // --- Thuộc tính Âm nhạc (Music) ---
    Music music;                 // Đối tượng Music (dùng cho nhạc nền - stream từ đĩa)
    bool musicLoaded = false;    // Cờ kiểm tra trạng thái tải nhạc nền.

    // --- Thuộc tính Hiệu ứng Âm thanh (SFX) ---
    // Khởi tạo các biến Sound ngay trong khai báo (Đảm bảo Raylib khởi tạo đúng cấu trúc và khắc phục cảnh báo C26495)
    Sound goalSFX = { 0 };       // Hiệu ứng âm thanh khi ghi bàn
    Sound missSFX = { 0 };       // Hiệu ứng âm thanh khi sút trượt/bị cản
    
public:
    // --- Constructor và Destructor ---
    // [KHAI BÁO CONSTRUCTOR] Cần thiết để khởi tạo các thành viên lớp nếu có logic phức tạp hơn
    SoundManager(); 

    // **LƯU Ý:** Nên thêm ~SoundManager() để Unload các tài nguyên (music, SFX) khi đối tượng bị hủy.

    // --- Phương thức Âm nhạc ---

    void LoadMusic(const char* path);
    // Tải nhạc nền từ đường dẫn file được chỉ định.

    // Hàm UpdateMusic cần được đặt ở đây.
    // PHẢI được gọi MỖI KHUNG HÌNH (frame) trong Game Loop để xử lý streaming (phát liên tục) của nhạc nền.
    void UpdateMusic(); 

    void PlayMusic(); // Bắt đầu phát nhạc nền đã tải.
    void ToggleMusic();
    // Chuyển đổi trạng thái phát nhạc (Play/Pause hoặc Stop/Play).
    bool IsMusicPlaying();
    // Trả về true nếu nhạc nền đang phát.

    // --- Phương thức Hiệu ứng Âm thanh ---

    void LoadSFX(const char* goalPath, const char* missPath);
    // Tải tất cả các file hiệu ứng âm thanh (SFX) cần thiết.

    void PlayGoalSFX();
    // Phát hiệu ứng âm thanh ghi bàn (thường là âm thanh ngắn, không cần update liên tục).
    void PlayMissSFX();
    // Phát hiệu ứng âm thanh sút trượt/bị cản.

    bool IsSFXPlaying();
    // Kiểm tra xem có bất kỳ hiệu ứng âm thanh nào đang phát không.
};
