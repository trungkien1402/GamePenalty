#include "menu.h"
#include "raylib.h"
#include "constants.h"

// Hàm vẽ chữ đậm (Thêm static để tránh lỗi link)
static void DrawTextBold(const char* text, int x, int y, int size, Color color) {
    // Tạo hiệu ứng viền (outline) màu đen bằng cách vẽ văn bản 4 lần dịch chuyển
    DrawText(text, x - 2, y, size, BLACK);
    DrawText(text, x + 2, y, size, BLACK);
    DrawText(text, x, y - 2, size, BLACK);
    DrawText(text, x, y + 2, size, BLACK);
    // Vẽ văn bản chính với màu mong muốn lên trên cùng
    DrawText(text, x, y, size, color);
}

// Biến tĩnh để lưu ảnh nền (chỉ load 1 lần, tránh lãng phí bộ nhớ VRAM)
static Texture2D texBackground;
static bool isBackgroundLoaded = false;

// Hàm cập nhật và vẽ Menu chính (Chạy trong Game Loop)
GameState UpdateMenu(GameState currentState) {
    // Lấy kích thước màn hình hiện tại (để tính toán vị trí, hỗ trợ màn hình co giãn)
    float screenWidth = (float)GetScreenWidth();
    float screenHeight = (float)GetScreenHeight();

    // Định nghĩa các mục Menu và tổng số lượng
    const char* items[] = { "PLAY GAME", "INSTRUCTIONS", "SOUND OPTION", "EXIT GAME" };
    const int count = 4;

    // Cấu hình kích thước và khoảng cách cố định cho các nút
    const int buttonW = 500;
    const int buttonH = 80;
    const int spacing = 110;
    const int fontSize = 40;

    // Tự động căn giữa Menu theo chiều ngang
    const int startX = (int)(screenWidth - buttonW) / 2;
    // Đặt vị trí bắt đầu Menu (StartY) ở 40% chiều cao màn hình
    const int startY = (int)(screenHeight * 0.4f); 

    // Lấy vị trí chuột hiện tại (cần thiết cho tương tác GUI)
    Vector2 mousePos = GetMousePosition();
    int selected = -1; // Chỉ số của nút đang được di chuột qua (hover)

    // --- PHASE 1: Xử lý Logic Input (Update) ---

    // 1. Logic kiểm tra chuột va chạm nút (Phát hiện trạng thái Hover)
    for (int i = 0; i < count; i++) {
        // Tạo hình chữ nhật đại diện cho vùng nút
        Rectangle r = { (float)startX, (float)(startY + i * spacing), (float)buttonW, (float)buttonH };
        // Kiểm tra xem vị trí chuột có nằm trong hình chữ nhật này không
        if (CheckCollisionPointRec(mousePos, r)) selected = i;
    }

    // 2. Logic bấm chuột (Xử lý sự kiện click)
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Nếu chuột được bấm và một nút được chọn (selected != -1)
        switch (selected) {
        case 0: return STATE_GAME;         // Bắt đầu game, trả về trạng thái Game Play
        case 1: return STATE_INSTRUCTIONS; // Chuyển sang màn hình hướng dẫn
        case 2: return STATE_SOUND;        // Chuyển sang màn hình cài đặt âm thanh
        case 3: return STATE_EXIT;         // Thoát khỏi Game Loop
        }
    }

    // 3. Load Background (Quản lý tài nguyên)
    // Kiểm tra cờ isBackgroundLoaded để đảm bảo LoadTexture chỉ được gọi MỘT LẦN
    if (!isBackgroundLoaded) {
        if (FileExists("Assets/backgroundmune.png")) {
            texBackground = LoadTexture("Assets/backgroundmune.png");
            isBackgroundLoaded = true;
        }
    }

    // --- PHASE 2: Vẽ Đồ họa (Draw) ---

    // 4. Vẽ Background
    if (isBackgroundLoaded) {
        // Vẽ ảnh nền (texture) co giãn để lấp đầy toàn bộ màn hình
        DrawTexturePro(
            texBackground,
            { 0, 0, (float)texBackground.width, (float)texBackground.height }, // Nguồn (Source): Lấy toàn bộ ảnh gốc
            { 0, 0, screenWidth, screenHeight },                              // Đích (Destination): Vẽ full màn hình
            { 0, 0 }, 0.0f, WHITE // Gốc xoay (origin), góc xoay (rotation), màu tô (tint)
        );
    }
    else {
        // Dự phòng: Nếu ảnh nền không tồn tại, chỉ cần vẽ một màu nền đồng nhất
        ClearBackground(DARKBLUE);
    }

    // 5. Vẽ Các Nút Bấm
    for (int i = 0; i < count; i++) {
        Rectangle btn = { (float)startX, (float)(startY + i * spacing), (float)buttonW, (float)buttonH };
        bool hover = (i == selected); // Kiểm tra trạng thái hover

        // Tính toán Hiệu ứng phóng to khi di chuột
        float scale = hover ? 1.05f : 1.0f; // Nếu hover, phóng to 5%
        Rectangle btnScaled = {
            // Điều chỉnh vị trí để giữ nút CĂN GIỮA khi phóng to
            btn.x - btn.width * (scale - 1) / 2,
            btn.y - btn.height * (scale - 1) / 2,
            btn.width * scale, // Kích thước mới
            btn.height * scale
        };

        // Vẽ nền nút (hình chữ nhật bo góc)
        Color btnColor = hover ? ORANGE : Fade(SKYBLUE, 0.8f);
        DrawRectangleRounded(btnScaled, 0.3f, 16, btnColor);

        // Vẽ viền nút (bo góc)
        DrawRectangleRoundedLines(btnScaled, 0.3f, 16, WHITE);

        // Tính toán vị trí để chữ được CĂN GIỮA bên trong nút
        int textWidth = MeasureText(items[i], fontSize);
        int textX = (int)(btnScaled.x + (btnScaled.width - textWidth) / 2);
        int textY = (int)(btnScaled.y + (btnScaled.height - fontSize) / 2);

        // Chọn màu chữ dựa trên trạng thái hover
        Color textColor = hover ? WHITE : DARKBLUE;
        DrawTextBold(items[i], textX, textY, fontSize, textColor);
    }

    // Dòng hướng dẫn nhỏ
    const char* hint = "Select an option to continue";
    int hintW = MeasureText(hint, 20);
    // Vẽ dòng hướng dẫn ở cuối màn hình
    DrawTextBold(hint, (int)(screenWidth - hintW) / 2, (int)(screenHeight - 50), 20, YELLOW);

    // Nếu không có nút nào được bấm, tiếp tục ở trạng thái Menu
    return currentState;
}
