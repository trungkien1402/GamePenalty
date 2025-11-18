#include "Player.h"

Player::Player()
{
    pos = { 500, 700 };
    isKicking = false;

    currentFrame = 0;
    frameSpeed = 0.03f; // tốc độ animation
    frameTimer = 0;
}

void Player::Load(const std::string& folder, int count)
{
    for (int i = 1; i <= count; i++)
    {
        char file[256];
        sprintf(file, "%s/kick_%04d.png", folder.c_str(), i);

        Texture2D tex = LoadTexture(file);
        frames.push_back(tex);
    }
}

void Player::Kick()
{
    isKicking = true;
    currentFrame = 0;
}

void Player::Update()
{
    if (!isKicking) return;

    frameTimer += GetFrameTime();
    if (frameTimer >= frameSpeed)
    {
        currentFrame++;
        frameTimer = 0;

        if (currentFrame >= frames.size()) {
            currentFrame = frames.size() - 1;
            isKicking = false;  // kết thúc animation
        }
    }
}

void Player::Draw()
{
    if (!frames.size()) return;

    Texture2D tex = frames[currentFrame];

    DrawTexture(tex, pos.x, pos.y, WHITE);
}

void Player::Unload()
{
    for (auto& f : frames) UnloadTexture(f);
}
