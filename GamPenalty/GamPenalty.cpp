#include "raylib.h"
#include <string>

enum GameState { STATE_MENU, STATE_GAME };

struct PlayerAnim {
    Texture2D frames[20];  // chứa tối đa 20 frame
    int frameCount = 0;
    int current = 0;
    float timer = 0;
    float frameSpeed = 0.07f;
    bool visible = false;  // chỉ hiện khi bấm SPACE
    Vector2 pos;
} player;

void LoadPlayerAnim() {
    // Ví dụ: bạn có 8 frame cầu thủ sút đã xóa nền: kick_0.png → kick_7.png
    player.frameCount = 8;
    for (int i = 0; i < player.frameCount; i++) {
        std::string file = "assets/kick_" + std::to_string(i) + ".png";
        player.frames[i] = LoadTexture(file.c_str());
    }

    player.pos = { 400, 500 };
}

void UpdatePlayerAnim(float dt) {
    if (!player.visible) return;

    player.timer += dt;
    if (player.timer >= player.frameSpeed) {
        player.timer = 0;
        player.current++;
        if (player.current >= player.frameCount) player.current = 0;
    }
}

void DrawPlayerAnim() {
    if (!player.visible) return;
    DrawTexture(player.frames[player.current], player.pos.x, player.pos.y, WHITE);
}

int main() {
    InitWindow(1200, 800, "Penalty Game");

    GameState state = STATE_MENU;

    LoadPlayerAnim();

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // ================= MENU =================
        if (state == STATE_MENU) {
            if (IsKeyPressed(KEY_ENTER)) {
                state = STATE_GAME;
            }
        }

        // ================= GAME =================
        if (state == STATE_GAME) {

            if (IsKeyPressed(KEY_ESCAPE))
                state = STATE_MENU;

            // ẤN SPACE → hiện cầu thủ
            if (IsKeyPressed(KEY_SPACE)) {
                player.visible = true;
                player.current = 0;
                player.timer = 0;
            }

            UpdatePlayerAnim(dt);
        }

        // ================= DRAW =================
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (state == STATE_MENU) {
            DrawText("MENU", 550, 200, 50, BLACK);
            DrawText("Press ENTER to PLAY", 500, 400, 30, DARKGRAY);

        }
        else if (state == STATE_GAME) {
            DrawText("GAME SCREEN", 520, 50, 40, BLACK);
            DrawText("Press SPACE to show player", 450, 120, 30, GRAY);
            DrawText("Press ESC to return to menu", 450, 160, 25, GRAY);

            DrawPlayerAnim();
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
