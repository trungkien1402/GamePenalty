#include "menu.h"
#include "raylib.h"

// Texture background
static Texture2D texBackground;
static bool isBackgroundLoaded = false;

GameState UpdateMenu(GameState currentState)
{
    const char* items[] = { "PLAY", "INSTRUCTIONS", "SOUND", "EXIT" };
    const int count = 4;

    const int startX = 450;
    const int startY = 350;
    const int buttonW = 300;
    const int buttonH = 60;
    const int spacing = 80;
    const int fontSize = 30;

    Vector2 mousePos = GetMousePosition();
    int selected = -1;

    for (int i = 0; i < count; i++)
    {
        Rectangle r = { (float)startX, (float)(startY + i * spacing), (float)buttonW, (float)buttonH };
        if (CheckCollisionPointRec(mousePos, r))
            selected = i;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        switch (selected)
        {
        case 0: return STATE_GAME;
        case 1: return STATE_INSTRUCTIONS;
        case 2: return STATE_SOUND;
        case 3: return STATE_EXIT;
        }
    }

    if (!isBackgroundLoaded)
    {
        texBackground = LoadTexture("Assets/menu_bg.png");
        isBackgroundLoaded = true;
    }

    DrawTexture(texBackground, 0, 0, WHITE);

    for (int i = 0; i < count; i++)
    {
        Rectangle btn = { (float)startX, (float)(startY + i * spacing), (float)buttonW, (float)buttonH };
        bool hover = (i == selected);

        float scale = hover ? 1.05f : 1.0f;
        Rectangle btnScaled = {
            btn.x - btn.width * (scale - 1) / 2,
            btn.y - btn.height * (scale - 1) / 2,
            btn.width * scale,
            btn.height * scale
        };

        if (hover)
            DrawRectangleRounded(btnScaled, 0.3f, 16, YELLOW);

        if (hover)
            DrawRectangleRoundedLines(btnScaled, 0.3f, 4, YELLOW);

        DrawRectangleRounded(btnScaled, 0.3f, 16, hover ? YELLOW : DARKGRAY);

        int textWidth = MeasureText(items[i], fontSize);
        int textX = btnScaled.x + (btnScaled.width - textWidth) / 2;
        int textY = btnScaled.y + (btnScaled.height - fontSize) / 2;
        DrawText(items[i], textX, textY, fontSize, hover ? BLACK : WHITE);
    }

    DrawText("Click button to select", 425, 850, fontSize, RED);

    return currentState;
}
