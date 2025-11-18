#include "raylib.h"
#include "menu.h"
#include "sound.h"

int main()
{
    InitWindow(1200, 900, "Penalty Kick 2D");

    InitAudioDevice();

    SoundManager soundManager;
    soundManager.LoadMusic("C:/VSCODE/GamPenalty/x64/Debug/Assets/nhacnen.mp3");
    soundManager.PlayMusic();

    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    GameState state = STATE_MENU;

    while (!WindowShouldClose())
    {
        soundManager.UpdateMusic();

        BeginDrawing();

        // LUÔN clear màn hình trước khi vẽ
        ClearBackground(BLACK);

        switch (state)
        {
        case STATE_MENU:
            state = UpdateMenu(state);
            break;

        case STATE_INSTRUCTIONS:
            ClearBackground(BLACK);
            DrawText("INSTRUCTIONS SCREEN", 420, 150, 50, WHITE);
            DrawText("Press ESC to return to MENU", 380, 300, 30, WHITE);

            if (IsKeyPressed(KEY_ESCAPE))
                state = STATE_MENU;
            break;

        case STATE_SOUND:
            ClearBackground(BLACK);
            DrawText("SOUND SETTINGS", 420, 150, 40, WHITE);

            if (soundManager.IsMusicPlaying())
                DrawText("Music: ON (Press SPACE)", 400, 250, 30, GREEN);
            else
                DrawText("Music: OFF (Press SPACE)", 400, 250, 30, RED);

            if (IsKeyPressed(KEY_SPACE))
                soundManager.ToggleMusic();

            if (IsKeyPressed(KEY_ESCAPE))
                state = STATE_MENU;

            break;

        case STATE_GAME:
            ClearBackground(DARKBLUE);
            DrawText("GAME SCREEN", 420, 350, 40, WHITE);

            if (IsKeyPressed(KEY_ESCAPE))
                state = STATE_MENU;
            break;

        case STATE_EXIT:
            state = STATE_MENU;
            break;
        }

        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
