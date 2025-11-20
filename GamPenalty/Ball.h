#pragma once
#include "raylib.h"

struct Ball {
    Vector2 pos;
    Vector2 vel;
    float radius;
    bool moving;

    void Reset();
    Ball();
};