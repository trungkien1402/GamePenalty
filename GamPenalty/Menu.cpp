#include "menu.h"
#include "raylib.h"

// Texture background
static Texture2D texBackground = { 0 };
static bool isBackgroundLoaded = false;

void UnloadMenuResources()
{
    if (isBackgroundLoaded && texBackground.id > 0)
    {
        UnloadTexture(texBackground);
        isBackgroundLoaded = false;
    }
}

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

    // Kiểm tra hover chuột
    for (int i = 0; i < count; i++)
    {
        Rectangle r = { (float)startX, (float)(startY + i * spacing), (float)buttonW, (float)buttonH };
        if (CheckCollisionPointRec(mousePos, r))
            selected = i;
    }

    // Click chuột để chọn
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        switch (selected)
        {
        case 0: return STATE_GAME;
        case 1: return STATE_INSTRUCTIONS;
        case 2: return STATE_SOUND;
        case 3: CloseWindow(); return STATE_EXIT; // EXIT thoát hẳn game
        }
    }
    // Load background 1 lần
    if (!isBackgroundLoaded)
    {
        texBackground = LoadTexture("C:/VSCODE/GamPenalty/x64/Debug/Assets/menu_bg.png"); // đường dẫn tương đối
        if (texBackground.id <= 0)
        {
            TraceLog(LOG_ERROR, "FAILED TO LOAD menu_bg.png! Check file path.");
        }
        isBackgroundLoaded = true;
    }

    // Vẽ background nếu hợp lệ
    if (texBackground.id > 0)
    {
        Rectangle sourceRec = { 0.0f, 0.0f, (float)texBackground.width, (float)texBackground.height };
        Rectangle destRec = { 0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight() };
        Vector2 origin = { 0, 0 };
        DrawTexturePro(texBackground, sourceRec, destRec, origin, 0.0f, WHITE);
    }
    else
    {
        ClearBackground(RAYWHITE); // fallback màu trắng nếu texture không load
    }

    // Vẽ nút với hover + phóng to
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

        // Glow khi hover
        if (hover)
            DrawRectangleRounded(btnScaled, 0.3f, 16, YELLOW);

        // Outline khi hover
        if (hover)
            DrawRectangleRoundedLines(btnScaled, 0.3f, 16, YELLOW);

        // Nút chính
        DrawRectangleRounded(btnScaled, 0.3f, 16, hover ? YELLOW : DARKGRAY);

        // Text căn giữa
        int textWidth = MeasureText(items[i], fontSize);
        int textX = btnScaled.x + (btnScaled.width - textWidth) / 2;
        int textY = btnScaled.y + (btnScaled.height - fontSize) / 2;
        DrawText(items[i], textX, textY, fontSize, hover ? BLACK : WHITE);
    }

    // Thông báo nhỏ phía dưới
    DrawText("Click button to select", 425, 850, fontSize, RED);

    return currentState;
}
