#include "menu.h"
#include "raylib.h"

// Hàm hỗ trợ vẽ chữ đậm
void DrawTextBold(const char* text, int x, int y, int size, Color color) {
    DrawText(text, x - 1, y, size, color);
    DrawText(text, x + 1, y, size, color);
    DrawText(text, x, y - 1, size, color);
    DrawText(text, x, y + 1, size, color);
    DrawText(text, x, y, size, color);
}

static Texture2D texBackground;
static bool isBackgroundLoaded = false;

GameState UpdateMenu(GameState currentState) {
    const char* items[] = { "PLAY", "INSTRUCTIONS", "SOUND", "EXIT" };
    const int count = 4;

    // Canh chỉnh menu ra giữa màn hình 1500
    const int buttonW = 500;
    const int startX = (1500 - buttonW) / 2;
    const int startY = 350;
    const int buttonH = 80;
    const int spacing = 100;
    const int fontSize = 35;

    Vector2 mousePos = GetMousePosition();
    int selected = -1;

    for (int i = 0; i < count; i++) {
        Rectangle r = { (float)startX, (float)(startY + i * spacing), (float)buttonW, (float)buttonH };
        if (CheckCollisionPointRec(mousePos, r)) selected = i;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        switch (selected) {
        case 0: return STATE_GAME;
        case 1: return STATE_INSTRUCTIONS;
        case 2: return STATE_SOUND;
        case 3: return STATE_EXIT;
        }
    }

    if (!isBackgroundLoaded) {
        // Load file từ thư mục Assets
        texBackground = LoadTexture("Assets/backgroundmune.png");
        isBackgroundLoaded = true;
    }

    DrawTexturePro(texBackground,
        { 0, 0, (float)texBackground.width, (float)texBackground.height },
        { 0, 0, 1500, 900 },
        { 0, 0 }, 0.0f, WHITE
    );

    for (int i = 0; i < count; i++) {
        Rectangle btn = { (float)startX, (float)(startY + i * spacing), (float)buttonW, (float)buttonH };
        bool hover = (i == selected);
        float scale = hover ? 1.07f : 1.0f;
        Rectangle btnScaled = {
            btn.x - btn.width * (scale - 1) / 2,
            btn.y - btn.height * (scale - 1) / 2,
            btn.width * scale,
            btn.height * scale
        };

        if (hover) DrawRectangleRounded(btnScaled, 0.5f, 16, YELLOW);
        if (hover) DrawRectangleRoundedLines(btnScaled, 0.5f, 16, YELLOW);
        DrawRectangleRounded(btnScaled, 0.5f, 16, hover ? YELLOW : BLUE);

        int textWidth = MeasureText(items[i], fontSize);
        int textX = btnScaled.x + (btnScaled.width - textWidth) / 2;
        int textY = btnScaled.y + (btnScaled.height - fontSize) / 2;
        DrawTextBold(items[i], textX, textY, fontSize, hover ? BLACK : WHITE);
    }

    DrawTextBold("Click button to select", 575, 850, fontSize, RED);
    return currentState;
}
