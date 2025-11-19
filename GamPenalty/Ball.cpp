// File: ball.cpp
#include "ball.h"
#include "constants.h" // Cần để lấy SCREEN_W, SCREEN_H

Ball::Ball() {
    Reset();
    radius = 18.0f;
    moving = false;
}

void Ball::Reset() {
    pos = { SCREEN_W / 2.0f, SCREEN_H - 150.0f };
    vel = { 0,0 };
    moving = false;
}
