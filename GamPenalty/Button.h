#pragma once
#include "raylib.h"

struct Button {
    Rectangle rect;
    const char* text;
    Color color;
};

void DrawButton(const Button& b);
