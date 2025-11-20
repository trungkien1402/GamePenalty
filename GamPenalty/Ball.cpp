<<<<<<< HEAD
﻿#include "ball.h"
=======
#include "ball.h"
>>>>>>> 9b706097fc97aff184be64f9d710b8c7030e988f
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
<<<<<<< HEAD
}
=======
}
>>>>>>> 9b706097fc97aff184be64f9d710b8c7030e988f
