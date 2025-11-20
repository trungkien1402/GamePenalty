#pragma once
#include "raylib.h"

// Cấu trúc Button mô tả một nút bấm đơn giản trong giao diện
struct Button {
    Rectangle rect;      // Vùng hình chữ nhật của nút (x, y, width, height)
    const char* text;    // Chuỗi hiển thị trên nút
    Color color;         // Màu nền của nút
};

// Hàm vẽ nút bấm lên màn hình
// Bao gồm nền, viền và chữ căn giữa
void DrawButton(const Button& b);
