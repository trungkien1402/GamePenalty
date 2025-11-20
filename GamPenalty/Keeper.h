<<<<<<< HEAD
﻿#pragma once
=======
#pragma once
>>>>>>> 9b706097fc97aff184be64f9d710b8c7030e988f
#include "raylib.h"

struct Keeper {
    Rectangle rect;
    float speed;
    int dir; // -1: Trái, 0: Đứng, 1: Phải
    float minX, maxX;

    Keeper();
    void Reset(float goalX, float goalW, float goalY, float goalH);
    void Update(float dt, bool ballMoving);
<<<<<<< HEAD
};
=======
};
>>>>>>> 9b706097fc97aff184be64f9d710b8c7030e988f
