#include "button.h"
#include "raylib.h"

// Hàm vẽ một nút bấm lên màn hình
void DrawButton(const Button& b) {

    // Vẽ hình chữ nhật nền của nút bấm
    DrawRectangleRec(b.rect, b.color);

    // Vẽ viền mờ 20% màu đen quanh nút để nhìn rõ hơn
    DrawRectangleLines(
        (int)b.rect.x, 
        (int)b.rect.y, 
        (int)b.rect.width, 
        (int)b.rect.height, 
        Fade(BLACK, 0.2f)
    );

    // Vẽ chữ nằm giữa nút
    // Tính vị trí chữ sao cho căn giữa theo chiều ngang và dọc
    DrawText(
        b.text,
        (int)(b.rect.x + (b.rect.width - MeasureText(b.text, 20)) / 2),  // căn giữa ngang
        (int)(b.rect.y + (b.rect.height - 20) / 2),                      // căn giữa dọc
        20,                                                              // cỡ chữ
        WHITE                                                            // màu chữ
    );
}
