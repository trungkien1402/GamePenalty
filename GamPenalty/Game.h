// File: game.h (ĐÃ SỬA)
#pragma once

#include "raylib.h"
#include "constants.h"
#include "ball.h"
#include "keeper.h"
#include "button.h"

class Game {
public:
    Game();  // Constructor
    ~Game(); // Destructor
    void Run(); // Vòng lặp game chính

private:
    // === HÀM KHỞI TẠO & GIẢI PHÓNG ===
    void InitGame();
    void Shutdown();
    void ResetGame(); // Dùng cho "Play Again"

    // === HÀM VÒNG LẶP CHÍNH ===
    void Update();
    void Draw();

    // === HÀM CẬP NHẬT TRẠNG THÁI ===
    void UpdateMenu();
    void UpdateInstructions();
    void UpdateGame();
    void UpdateResult();

    // === HÀM VẼ TRẠNG THÁI ===
    void DrawMenu();
    void DrawInstructions();
    void DrawGame();
    void DrawResult();

    // === BIẾN THÀNH VIÊN (toàn bộ biến từ main cũ) ===
    GameState state;
    Vector2 mouse;
    float dt;

    // Assets
    Texture2D texGoal, texKeeper, texBall, texGrass;
    bool hasGoal, hasKeeper, hasBall, hasGrass;
    bool soundOn;

    // Buttons
    Button btnStart, btnInstr, btnSound, btnExit, btnPlayAgain, btnMenu;

    // Game Objects
    Rectangle goal;
    Ball ball;
    Keeper keeper;

    // Game State
    int shots;
    int goalsCount;
    bool nextShotReady;
    bool goal_scored;
    bool shotFailed;
    bool shotIsOverBar;

    // ==========================================================
    // *** THÊM CỜ HIỆU MỚI (SỬA LỖI SÚT CHÉO) ***
    bool shotIsOutWide; // (Sút ra ngoài cột dọc)
    // ==========================================================

    // Aim Logic
    ShotState shotState;
    Vector2 aimStartPos;
    Vector2 aimReticle;
    float aimRadius;
    float aimWobble;
    float aimTimer;

    // Hằng số logic sút (có thể để ở đây hoặc constants.h)
    const float AIM_SPEED_Y = 400.0f;
    const float AIM_SPEED_X = 400.0f;
    const float MAX_WOBBLE = 12.0f;
    const float POWER_BASE = 950.0f;
    const float AIM_TIMER_DURATION = 1.0f;
};
