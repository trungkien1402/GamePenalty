#include "raylib.h"
#include "raymath.h"

#include "menu.h"
#include "sound.h"
#include "player.h"

int main()
{
    InitWindow(1200, 900, "Penalty Kick 2D");
    InitAudioDevice();

    SetTargetFPS(60);
    SetExitKey(KEY_NULL);   // ESC không tắt game

    // ===================== NHẠC NỀN =====================
    SoundManager soundManager;
    soundManager.LoadMusic("Assets/music.mp3");
    soundManager.PlayMusic();

    // ===================== CẦU THỦ =====================
    Player player;
    player.Load("Assets/player", 40);   // load 40 frame animation

    GameState state = STATE_MENU;

    while (!WindowShouldClose())
    {
        soundManager.UpdateMusic();
        Vector2 mouse = GetMousePosition();

        // ===================== GAME UPDATE =====================
        if (state == STATE_GAME)
        {
            // bấm chuột để chạy animation sút
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                player.Kick();

            player.Update();
        }

        // ===================== VẼ =====================
        BeginDrawing();

        switch (state)
        {
        case STATE_MENU:
            ClearBackground(BLACK);
            state = UpdateMenu(state);
            break;

        case STATE_INSTRUCTIONS:
            ClearBackground(BLUE);
            DrawText("INSTRUCTIONS SCREEN", 420, 150, 50, WHITE);
            DrawText("Click ESC to return to MENU", 380, 300, 30, WHITE);
            if (IsKeyPressed(KEY_ESCAPE)) state = STATE_MENU;
            break;

        case STATE_SOUND:
            ClearBackground(DARKGRAY);
            DrawText("SOUND SETTINGS", 420, 150, 40, WHITE);

            if (soundManager.IsMusicPlaying())
                DrawText("Music: ON (Press SPACE)", 400, 250, 30, GREEN);
            else
                DrawText("Music: OFF (Press SPACE)", 400, 250, 30, RED);

            if (IsKeyPressed(KEY_SPACE))
                soundManager.ToggleMusic();

            DrawText("Press ESC to return", 460, 470, 25, WHITE);
            if (IsKeyPressed(KEY_ESCAPE)) state = STATE_MENU;
            break;

        case STATE_GAME:
            ClearBackground(RAYWHITE);

            // vẽ cầu thủ
            player.Draw();

            DrawText("GAME SCREEN", 450, 50, 40, BLACK);
            DrawText("Click to KICK", 500, 120, 25, BLACK);
            DrawText("ESC to return MENU", 480, 160, 25, BLACK);

            if (IsKeyPressed(KEY_ESCAPE))
                state = STATE_MENU;

            break;

        case STATE_EXIT:
            CloseWindow();
            return 0;
        }

        EndDrawing();
    }

    // ===================== GIẢI PHÓNG =====================
    player.Unload();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
