// File: keeper.h (Cập nhật)
#pragma once
#include "raylib.h"

struct Keeper {
    Rectangle rect;
    float speed;
    int dir; // -1: Trái, 0: Giữa, 1: Phải
    float minX, maxX;

    // SỬA: Thêm "float goalH" để biết chiều cao gôn
    void Reset(float goalX, float goalW, float goalY, float goalH);
    void Update(float dt, bool ballMoving);
    Keeper();
};
