#include "ball.h"
#include "constants.h"

Ball::Ball() {
    Reset();
    radius = 18.0f;//bán kính
    moving = false;
}

void Ball::Reset() {
    pos = { SCREEN_W / 2.0f, SCREEN_H - 150.0f };
    vel = { 0,0 };//vận tốc
    moving = false;
}
