#include "raylib.h"
#include "constants.h"
#include "GameState.h"
#include "menu.h"
#include "sound.h"
#include "game.h"

int main()
{
    InitWindow(SCREEN_W, SCREEN_H, "Penalty Kick 2D - Merged");
    InitAudioDevice();

    SoundManager soundManager;
    soundManager.LoadMusic("Assets/nhacnen.mp3");
    soundManager.PlayMusic();

    Game gameInstance;
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    GameState state = STATE_MENU;
    bool gameInitialized = false;

    while (!WindowShouldClose() && state != STATE_EXIT)
    {
        soundManager.UpdateMusic();

        switch (state)
        {
        case STATE_MENU:
            if (gameInitialized) {
                gameInstance.UnloadGame();
                gameInitialized = false;
            }
            state = UpdateMenu(state);
            break;

        case STATE_GAME:
            if (!gameInitialized) {
                gameInstance.InitGame();
                gameInitialized = true;
            }
            gameInstance.Update(state);
            break;

        case STATE_INSTRUCTIONS:
            if (IsKeyPressed(KEY_ESCAPE)) state = STATE_MENU;
            break;
        case STATE_SOUND:
            if (IsKeyPressed(KEY_SPACE)) soundManager.ToggleMusic();
            if (IsKeyPressed(KEY_ESCAPE)) state = STATE_MENU;
            break;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        switch (state)
        {
        case STATE_MENU: break;
        case STATE_GAME: gameInstance.Draw(); break;
        case STATE_INSTRUCTIONS:
            ClearBackground(BLACK);
            DrawText("INSTRUCTIONS", SCREEN_W / 2 - 150, 100, 50, YELLOW);
            DrawText("- Hold 'D' to aim height, Release to lock.", 400, 300, 30, WHITE);
            DrawText("- Use Arrow Left/Right to aim direction.", 400, 350, 30, WHITE);
            DrawText("Press ESC to return", SCREEN_W / 2 - 100, 800, 20, GRAY);
            break;
        case STATE_SOUND:
            ClearBackground(BLACK);
            DrawText("SOUND SETTINGS", SCREEN_W / 2 - 150, 150, 40, WHITE);
            if (soundManager.IsMusicPlaying()) DrawText("Music: ON (Press SPACE)", SCREEN_W / 2 - 180, 300, 30, GREEN);
            else DrawText("Music: OFF (Press SPACE)", SCREEN_W / 2 - 180, 300, 30, RED);
            DrawText("Press ESC to return", SCREEN_W / 2 - 100, 800, 20, GRAY);
            break;
        }
        EndDrawing();
    }

    gameInstance.UnloadGame();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
