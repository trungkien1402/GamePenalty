
#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include <vector>
#include <string>

class Player {
public:
    Vector2 pos;
    bool isKicking;

    std::vector<Texture2D> frames;
    int currentFrame;
    float frameSpeed;
    float frameTimer;

    Player();
    void Load(const std::string& folder, int count);
    void Update();
    void Draw();
    void Kick();  // bắt đầu animation sút
    void Unload();
};

#endif
