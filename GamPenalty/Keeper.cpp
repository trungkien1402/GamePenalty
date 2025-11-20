<<<<<<< HEAD
﻿#include "keeper.h"
=======
#include "keeper.h"
>>>>>>> 9b706097fc97aff184be64f9d710b8c7030e988f
#include "raylib.h"
#include <cmath>

// ============================================================
// *** BẢNG ĐIỀU KHIỂN TỌA ĐỘ (KÍCH THƯỚC 190x190) ***
// ============================================================

// 1. KÍCH THƯỚC LÚC ĐỨNG
const float SIZE_STAND_W = 190.0f;
const float SIZE_STAND_H = 190.0f;

// 2. VỊ TRÍ ĐỨNG DỌC (Y)
const float KEEPER_Y_POS = 240.0f;

// 3. VỊ TRÍ ĐỨNG NGANG (X) - SỬA SỐ NÀY LÀ THỦ MÔN DỊCH THEO
// 750 (Tâm màn hình) - 95 (Nửa người) = 655
const float KEEPER_X_POS = 663.0f;

// 4. KÍCH THƯỚC LÚC BAY (NẰM)
const float SIZE_DIVE_W = 190.0f;
const float SIZE_DIVE_H = 190.0f;

// 5. BIÊN CHẠY
const float LIMIT_LEFT = 330.0f;
const float LIMIT_RIGHT = 940.0f;

// ============================================================

Keeper::Keeper() {
    rect = { 0, 0, SIZE_STAND_W, SIZE_STAND_H };
    speed = 580.0f;
    dir = 1;
}

void Keeper::Reset(float goalX, float goalW, float goalY, float goalH) {
    rect.x = KEEPER_X_POS;
    rect.y = KEEPER_Y_POS;
    rect.width = SIZE_STAND_W;
    rect.height = SIZE_STAND_H;

    minX = LIMIT_LEFT;
    maxX = LIMIT_RIGHT;

    dir = (GetRandomValue(0, 1) == 0) ? -1 : 1;
}

void Keeper::Update(float dt, bool ballMoving) {
    if (ballMoving) {
        // --- TRẠNG THÁI: BAY NGƯỜI ---
        if (dir != 0) {
            rect.width = SIZE_DIVE_W;
            rect.height = SIZE_DIVE_H;

            // Hạ thấp trọng tâm
            rect.y = KEEPER_Y_POS + (SIZE_STAND_H - SIZE_DIVE_H);

            rect.x += dir * speed * dt;

            if (rect.x < minX) { rect.x = minX; dir = 1; }

            float limitRightDive = maxX;
            if (rect.x > limitRightDive) { rect.x = limitRightDive; dir = -1; }
        }
        else {
            // Đứng yên
            rect.width = SIZE_STAND_W;
            rect.height = SIZE_STAND_H;
            rect.y = KEEPER_Y_POS;
        }
    }
    else {
        // --- TRẠNG THÁI: ĐỨNG CHỜ ---

        rect.width = SIZE_STAND_W;
        rect.height = SIZE_STAND_H;
        rect.y = KEEPER_Y_POS;

        // [QUAN TRỌNG] SỬA LỖI TẠI ĐÂY
        // Thay vì tính trung bình cộng (min+max)/2, ta bắt nó về đúng KEEPER_X_POS
        float centerX = KEEPER_X_POS;

        float move = speed * dt;

        // Logic di chuyển về vị trí gốc
        if (fabs(rect.x - centerX) <= move) rect.x = centerX;
        else if (rect.x < centerX) rect.x += move;
        else rect.x -= move;
    }
<<<<<<< HEAD
}
=======
}
>>>>>>> 9b706097fc97aff184be64f9d710b8c7030e988f
