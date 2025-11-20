#include "menu.h"
#include "raylib.h"
#include "constants.h"

// Hàm vẽ chữ đậm (Thêm static để tránh lỗi link)
static void DrawTextBold(const char* text, int x, int y, int size, Color color) {
    DrawText(text, x - 2, y, size, BLACK);
    DrawText(text, x + 2, y, size, BLACK);
    DrawText(text, x, y - 2, size, BLACK);
    DrawText(text, x, y + 2, size, BLACK);
    DrawText(text, x, y, size, color);
}

// Biến tĩnh để lưu ảnh nền (chỉ load 1 lần)
static Texture2D texBackground;
static bool isBackgroundLoaded = false;

GameState UpdateMenu(GameState currentState) {
    // Lấy kích thước màn hình hiện tại (để tính toán vị trí)
    float screenWidth = (float)GetScreenWidth();
    float screenHeight = (float)GetScreenHeight();

    const char* items[] = { "PLAY GAME", "INSTRUCTIONS", "SOUND OPTION", "EXIT GAME" };
    const int count = 4;

    // Cấu hình nút bấm
    const int buttonW = 500;
    const int buttonH = 80;
    const int spacing = 110;
    const int fontSize = 40;

    // Tự động căn giữa menu
    const int startX = (int)(screenWidth - buttonW) / 2;
    const int startY = (int)(screenHeight * 0.4f); // Menu bắt đầu từ 40% chiều cao màn hình

    Vector2 mousePos = GetMousePosition();
    int selected = -1;

    // 1. Logic kiểm tra chuột va chạm nút
    for (int i = 0; i < count; i++) {
        Rectangle r = { (float)startX, (float)(startY + i * spacing), (float)buttonW, (float)buttonH };
        if (CheckCollisionPointRec(mousePos, r)) selected = i;
    }

    // 2. Logic bấm chuột
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        switch (selected) {
        case 0: return STATE_GAME;
        case 1: return STATE_INSTRUCTIONS;
        case 2: return STATE_SOUND;
        case 3: return STATE_EXIT;
        }
    }

    // 3. Load Background (Chỉ chạy 1 lần đầu tiên)
    if (!isBackgroundLoaded) {
        if (FileExists("Assets/backgroundmune.png")) {
            texBackground = LoadTexture("Assets/backgroundmune.png");
            isBackgroundLoaded = true;
        }
    }

    // 4. Vẽ Background
    if (isBackgroundLoaded) {
        // Vẽ ảnh background co giãn full màn hình
        DrawTexturePro(
            texBackground,
            { 0, 0, (float)texBackground.width, (float)texBackground.height }, // Nguồn: Lấy hết ảnh
            { 0, 0, screenWidth, screenHeight },                               // Đích: Full màn hình
            { 0, 0 }, 0.0f, WHITE
        );
    }
    else {
        // Dự phòng: Nếu chưa có ảnh thì vẽ màu xanh
        ClearBackground(DARKBLUE);
    }

    // 5. Vẽ Các Nút Bấm
    for (int i = 0; i < count; i++) {
        Rectangle btn = { (float)startX, (float)(startY + i * spacing), (float)buttonW, (float)buttonH };
        bool hover = (i == selected);

        // Hiệu ứng phóng to khi di chuột
        float scale = hover ? 1.05f : 1.0f;
        Rectangle btnScaled = {
            btn.x - btn.width * (scale - 1) / 2,
            btn.y - btn.height * (scale - 1) / 2,
            btn.width * scale,
            btn.height * scale
        };

        // Màu nút
        Color btnColor = hover ? ORANGE : Fade(SKYBLUE, 0.8f);
        DrawRectangleRounded(btnScaled, 0.3f, 16, btnColor);

        // [ĐÃ SỬA] Xóa tham số độ dày (số 3) để tương thích với Raylib cũ
        DrawRectangleRoundedLines(btnScaled, 0.3f, 16, WHITE);

        // Vẽ chữ
        int textWidth = MeasureText(items[i], fontSize);
        int textX = (int)(btnScaled.x + (btnScaled.width - textWidth) / 2);
        int textY = (int)(btnScaled.y + (btnScaled.height - fontSize) / 2);

        // Chọn màu chữ
        Color textColor = hover ? WHITE : DARKBLUE;
        DrawTextBold(items[i], textX, textY, fontSize, textColor);
    }

    // Dòng hướng dẫn nhỏ
    const char* hint = "Select an option to continue";
    int hintW = MeasureText(hint, 20);
    DrawTextBold(hint, (int)(screenWidth - hintW) / 2, (int)(screenHeight - 50), 20, YELLOW);

    return currentState;
}