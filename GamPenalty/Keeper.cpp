#include "keeper.h"
#include "raylib.h"
#include <cmath>

//ĐIỀU CHỈNH THỦ MÔN

// kích thước đứng
//chiều rộng
const float SIZE_STAND_W = 190.0f;
//chiều cao
const float SIZE_STAND_H = 190.0f;

//Vị trí thủ môn, lên xuống theo trục y
const float KEEPER_Y_POS = 240.0f;
//Vị trí thủ môn, lên xuống theo trục x
const float KEEPER_X_POS = 663.0f;

//Kích thức lúc bay
const float SIZE_DIVE_W = 190.0f;
const float SIZE_DIVE_H = 190.0f;

//giới hạn sang trái rồi trở lại giữa
const float LIMIT_LEFT = 330.0f;
//giới hạn sang phải rồi trở lại giữa
const float LIMIT_RIGHT = 940.0f;


Keeper::Keeper() {
    //tỉ lệ thủ môn
    rect = { 0, 0, SIZE_STAND_W, SIZE_STAND_H };
    //tốc độ bay
    speed = 580.0f;

    dir = 1;
}

// Hàm resert thủ môn về vị trí cb bắt
void Keeper::Reset(float goalX, float goalW, float goalY, float goalH) {
    rect.x = KEEPER_X_POS;
    rect.y = KEEPER_Y_POS;
    rect.width = SIZE_STAND_W;
    rect.height = SIZE_STAND_H;
    minX = LIMIT_LEFT;
    maxX = LIMIT_RIGHT;

    //ramdom hướng bay
    dir = (GetRandomValue(0, 1) == 0) ? -1 : 1;
}
// điều khiển hành vi và vị trí gk dựa vào việc bóng bay hay dừng
void Keeper::Update(float dt, bool ballMoving) {
    if (ballMoving) {
        if (dir != 0) {
            rect.width = SIZE_DIVE_W;
            rect.height = SIZE_DIVE_H;

            // Hạ thấp trọng tâm
            rect.y = KEEPER_Y_POS + (SIZE_STAND_H - SIZE_DIVE_H);
            //di chuyển bay ngang
            rect.x += dir * speed * dt;

            //chạm giới hạn biên trái, dừng lại ở biên đổi hướng di chuyển ngc lại
            if (rect.x < minX) { rect.x = minX; dir = 1; }
            //tương tự biên phải
            float limitRightDive = maxX;
            if (rect.x > limitRightDive) { rect.x = limitRightDive; dir = -1; }
        }
        else {
            // nếu dir=0 nên đứng giữa 
            rect.width = SIZE_STAND_W;
            rect.height = SIZE_STAND_H;
            rect.y = KEEPER_Y_POS;
        }
    }
    else {
        // Chờ nếu bóng k mu
        rect.width = SIZE_STAND_W;
        rect.height = SIZE_STAND_H;
        rect.y = KEEPER_Y_POS;

        // quay về trung tâm sau khi sút xong 1 lượt
        float centerX = KEEPER_X_POS;
        float move = speed * dt;

        // Logic di chuyển về vị trí gốc
        if (fabs(rect.x - centerX) <= move) rect.x = centerX;
        else if (rect.x < centerX) rect.x += move;
        else rect.x -= move;
    }
}
