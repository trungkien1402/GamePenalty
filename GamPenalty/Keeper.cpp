// File: keeper.cpp (Cập nhật)
#include "keeper.h"
#include "raylib.h" // Cần cho GetRandomValue
#include <cmath>     // Cần cho fabs

Keeper::Keeper() {
    rect = { 0,0,80,80 }; // Kích thước thủ môn (cao 80)
    speed = 220.0f;
    dir = 1;
}

// SỬA: Thêm "float goalH"
void Keeper::Reset(float goalX, float goalW, float goalY, float goalH) {
    // Đặt giữa theo chiều ngang
    rect.x = 560.0f;

    // ==========================================================
    // *** SỬA VỊ TRÍ THỦ MÔN (Đứng trên đáy gôn) ***
    //
    // Tính toán đáy của gôn (logic)
    float logicGoalBottom = goalY + goalH; // (Vd: 100 + 140 = 240)
    //
    // Đặt thủ môn đứng trên đáy gôn
    // (Vị trí Y = Đáy gôn - Chiều cao thủ môn - 5 pixel đệm cho đẹp)
    rect.y = 220.0f; // (Vd: 240 - 80 - 5 = 155)
    // ==========================================================

    // Tính biên di chuyển
    minX = goalX + 20.0f;
    maxX = goalX + goalW - rect.width - 20.0f;

    // Reset hướng bay ngẫu nhiên
    dir = (GetRandomValue(0, 1) == 0) ? -1 : 1;
}

// Hàm cập nhật di chuyển của thủ môn
void Keeper::Update(float dt, bool ballMoving) {
    if (ballMoving) {
        // Nếu bóng đang bay, thủ môn di chuyển theo hướng đã chọn (dir)
        if (dir != 0) { // dir = 0 là đứng giữa
            rect.x += dir * speed * dt;
            if (rect.x < minX) { rect.x = minX; dir = 1; } // Chạm biên trái
            if (rect.x > maxX) { rect.x = maxX; dir = -1; } // Chạm biên phải
        }
    }
    else {
        // Nếu bóng chưa bay, thủ môn từ từ quay về giữa gôn
        float centerX = (minX + maxX) * 0.5f;
        float move = speed * dt;
        if (fabs(rect.x - centerX) <= move) rect.x = centerX;
        else if (rect.x < centerX) rect.x += move;
        else rect.x -= move;
    }
}
