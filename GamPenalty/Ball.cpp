#include "ball.h"
#include "constants.h"

Ball::Ball() {
    Reset();
    radius = 18.0f;
    moving = false;
}

void Ball::Reset() {
    // Căn giữa bóng theo màn hình 1500px
    pos = { SCREEN_W / 2.0f, SCREEN_H - 150.0f };
    vel = { 0,0 };
    moving = false;
}
