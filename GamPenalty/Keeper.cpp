#include "keeper.h"
#include "raylib.h"
#include <cmath>

Keeper::Keeper() {
    rect = { 0,0,80,80 };
    speed = 220.0f;
    dir = 1;
}

void Keeper::Reset(float goalX, float goalW, float goalY, float goalH) {
    rect.x = goalX + (goalW - rect.width) / 2.0f; // Đứng giữa gôn
    rect.y = goalY + goalH - rect.height - 5.0f; // Đứng trên vạch vôi

    minX = goalX + 20.0f;
    maxX = goalX + goalW - rect.width - 20.0f;

    dir = (GetRandomValue(0, 1) == 0) ? -1 : 1;
}

void Keeper::Update(float dt, bool ballMoving) {
    if (ballMoving) {
        if (dir != 0) {
            rect.x += dir * speed * dt;
            if (rect.x < minX) { rect.x = minX; dir = 1; }
            if (rect.x > maxX) { rect.x = maxX; dir = -1; }
        }
    }
    else {
        // Quay về giữa khi bóng chưa sút
        float centerX = (minX + maxX) * 0.5f;
        float move = speed * dt;
        if (fabs(rect.x - centerX) <= move) rect.x = centerX;
        else if (rect.x < centerX) rect.x += move;
        else rect.x -= move;
    }
}
