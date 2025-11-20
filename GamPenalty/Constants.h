#pragma once

// -------------------------------
// Cấu hình màn hình cho Menu/Game
// -------------------------------

// Kích thước cửa sổ game (Menu rộng hơn nên chọn 1500x900)
#define SCREEN_W 1500
#define SCREEN_H 900

// Số lần sút tối đa trong gameplay
#define MAX_SHOTS 7

// -------------------------------
// Trạng thái nội bộ trong gameplay
// -------------------------------

// Trạng thái điều khiển logic bên trong khi đang chơi game
enum InternalState {
    INTERNAL_PLAYING,   // Đang chơi, đang sút
    INTERNAL_RESULT     // Đã sút xong → chuyển sang hiển thị kết quả
};

// -------------------------------
// Trạng thái của một cú sút penalty
// -------------------------------

enum ShotState {
    STATE_READY,            // Chưa bắt đầu → chờ người chơi nhấn phím
    STATE_AIM_VERTICAL,     // Đang canh lực / hướng theo trục dọc (lên - xuống)
    STATE_AIM_HORIZONTAL,   // Đang canh hướng theo trục ngang (trái - phải)
    STATE_FIRED             // Đã sút bóng → đang xử lý chuyển động
};

