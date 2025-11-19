// File: game.cpp (ĐÃ SỬA LỖI SÚT CHÉO)

#include "game.h"
#include <cstdio>  // Dùng cho sprintf_s
#include <cmath>   // Dùng cho fabs, fmaxf, ...

// === Constructor: Khởi tạo cửa sổ và Game ===
Game::Game() {
    InitWindow(SCREEN_W, SCREEN_H, "Penalty Kick 2D - Refactored");
    InitGame();
}

// === Destructor: Giải phóng tài nguyên ===
Game::~Game() {
    Shutdown();
    CloseWindow();
}

// === Khởi tạo Game (Load assets, tạo buttons) ===
void Game::InitGame() {
    const char* ASSET_GOAL = "Assets/gon.png";
    const char* ASSET_KEEPER = "Assets/keeper.png";
    const char* ASSET_BALL = "Assets/ball.png";
    const char* ASSET_GRASS = "Assets/grass.png";

    hasGoal = FileExists(ASSET_GOAL);
    hasKeeper = FileExists(ASSET_KEEPER);
    hasBall = FileExists(ASSET_BALL);
    hasGrass = FileExists(ASSET_GRASS);
    if (hasGoal) texGoal = LoadTexture(ASSET_GOAL);
    if (hasKeeper) texKeeper = LoadTexture(ASSET_KEEPER);
    if (hasBall) texBall = LoadTexture(ASSET_BALL);
    if (hasGrass) texGrass = LoadTexture(ASSET_GRASS);

    // BUTTONS
    float BTN_W = 260;
    float BTN_H = 55;
    float centerX = (SCREEN_W - BTN_W) / 2.0f;
    btnStart = { { centerX, 250, BTN_W, BTN_H }, "Start Game", DARKGREEN };
    btnInstr = { { centerX, 320, BTN_W, BTN_H }, "Instructions", DARKBLUE };
    btnSound = { { centerX, 390, BTN_W, BTN_H }, "Sound: ON", DARKGRAY };
    btnExit = { { centerX, 460, BTN_W, BTN_H }, "Exit", RED };
    btnPlayAgain = { { centerX, 350, BTN_W, BTN_H }, "Play Again", DARKGREEN };
    btnMenu = { { centerX, 420, BTN_W, BTN_H }, "Menu", DARKBLUE };

    // KHU VỰC LOGIC TÍNH ĐIỂM (Tọa độ của bạn)
    float logicX = (SCREEN_W - 400.0f) / 2.0f + 20.0f;
    float logicY = 100.0f;
    float logicW = 360.0f;
    float logicH = 140.0f;

    goal = { logicX, logicY, logicW, logicH };

    // Truyền logic gôn vào cho thủ môn
    keeper.Reset(goal.x, goal.width, goal.y, goal.height);

    aimStartPos = { SCREEN_W / 2.0f, goal.y + goal.height + 30.0f };

    soundOn = true;
    state = STATE_MENU;

    // Reset trạng thái game
    ResetGame();
}

// === Giải phóng (Unload textures) ===
void Game::Shutdown() {
    if (hasGoal) UnloadTexture(texGoal);
    if (hasKeeper) UnloadTexture(texKeeper);
    if (hasBall) UnloadTexture(texBall);
    if (hasGrass) UnloadTexture(texGrass);
}

// === Reset khi chơi lại ===
void Game::ResetGame() {
    shots = 0;
    goalsCount = 0;
    ball.Reset();
    keeper.Reset(goal.x, goal.width, goal.y, goal.height);
    aimReticle = aimStartPos;
    shotState = STATE_READY;
    nextShotReady = true;
    aimWobble = 0.0f;
    aimTimer = 0.0f;
    goal_scored = false;
    shotFailed = false;
    shotIsOverBar = false;

    // ==========================================================
    // *** SỬA LỖI SÚT CHÉO (BƯỚC 1): Reset cờ hiệu ***
    shotIsOutWide = false;
    // ==========================================================
}

// === Vòng lặp chính ===
void Game::Run() {
    SetTargetFPS(60);
    while (!WindowShouldClose() && state != STATE_EXIT) {
        Update(); // Cập nhật logic
        Draw();   // Vẽ
    }
}

// === Cập nhật logic (phân luồng) ===
void Game::Update() {
    dt = GetFrameTime();
    mouse = GetMousePosition();

    switch (state) {
    case STATE_MENU:         UpdateMenu(); break;
    case STATE_INSTRUCTIONS: UpdateInstructions(); break;
    case STATE_GAME:         UpdateGame(); break;
    case STATE_RESULT:       UpdateResult(); break;
    default: break;
    }
}

// === Vẽ (phân luồng) ===
void Game::Draw() {
    BeginDrawing();
    ClearBackground(BLACK); // Nền đen mặc định

    switch (state) {
    case STATE_MENU:         DrawMenu(); break;
    case STATE_INSTRUCTIONS: DrawInstructions(); break;
    case STATE_GAME:         DrawGame(); break;
    case STATE_RESULT:       DrawResult(); break;
    default: break;
    }

    EndDrawing();
}

// === CÁC HÀM UPDATE CHI TIẾT ===

void Game::UpdateMenu() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mouse, btnStart.rect)) {
            state = STATE_GAME;
            ResetGame();
        }
        else if (CheckCollisionPointRec(mouse, btnInstr.rect)) state = STATE_INSTRUCTIONS;
        else if (CheckCollisionPointRec(mouse, btnSound.rect)) {
            soundOn = !soundOn;
            btnSound.text = soundOn ? "Sound: ON" : "Sound: OFF";
        }
        else if (CheckCollisionPointRec(mouse, btnExit.rect)) state = STATE_EXIT;
    }
}

void Game::UpdateInstructions() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        state = STATE_MENU;
    }
}

void Game::UpdateGame() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        state = STATE_MENU;
        return;
    }

    // ----- LOGIC SÚT MỚI -----
    if (shotState == STATE_READY && nextShotReady) {
        aimReticle = aimStartPos;
        if (IsKeyPressed(KEY_D)) {
            shotState = STATE_AIM_VERTICAL;
            nextShotReady = false;
            goal_scored = false;
            shotFailed = false;
            shotIsOverBar = false;

            // ==========================================================
            // *** SỬA LỖI SÚT CHÉO (BƯỚC 2): Reset cờ hiệu ***
            shotIsOutWide = false;
            // ==========================================================

            ball.Reset();
        }
    }

    if (shotState == STATE_AIM_VERTICAL) {
        if (IsKeyDown(KEY_D)) {
            aimReticle.y -= AIM_SPEED_Y * dt;
            aimReticle.y = fmaxf(goal.y - 50, aimReticle.y);
        }
        if (IsKeyReleased(KEY_D)) {
            shotState = STATE_AIM_HORIZONTAL;
            aimReticle.x = aimStartPos.x;
            aimWobble = 0.0f;
            aimTimer = AIM_TIMER_DURATION;
        }
    }

    if (shotState == STATE_AIM_HORIZONTAL) {
        aimTimer -= dt;
        if (IsKeyDown(KEY_LEFT)) aimReticle.x -= AIM_SPEED_X * dt;
        if (IsKeyDown(KEY_RIGHT)) aimReticle.x += AIM_SPEED_X * dt;

        aimReticle.x = fmaxf(goal.x - 20, fminf(aimReticle.x, goal.x + goal.width + 20));

        aimRadius = 15.0f; // Đặt bán kính tâm ngắm cố định

        if (aimTimer <= 0.0f) {
            shotState = STATE_FIRED;
        }
    }

    if (shotState == STATE_FIRED && !ball.moving) {
        ball.moving = true;
        shots++;
        keeper.dir = GetRandomValue(0, 2) - 1;

        Vector2 target = aimReticle;

        // ==========================================================
        // *** SỬA LỖI SÚT CHÉO (BƯỚC 3): Kiểm tra tâm ngắm ***
        //
        // 1. Kiểm tra Lên trời (Xà ngang)
        shotIsOverBar = (aimReticle.y < goal.y);
        //
        // 2. Kiểm tra Ra ngoài (Cột dọc)
        shotIsOutWide = (aimReticle.x < goal.x || aimReticle.x > goal.x + goal.width);
        // ==========================================================

        if (shotIsOverBar) {
            target = { aimReticle.x + GetRandomValue(-50, 50), goal.y - 300 };
        }

        // (Đã xóa Wobble)

        float angle = atan2f(target.y - ball.pos.y, target.x - ball.pos.x);
        ball.vel.x = cosf(angle) * POWER_BASE;
        ball.vel.y = sinf(angle) * POWER_BASE;
    }

    // KEEPER UPDATE
    keeper.Update(dt, ball.moving);

    // BALL UPDATE
    if (ball.moving) {
        ball.pos.x += ball.vel.x * dt;
        ball.pos.y += ball.vel.y * dt;

        float nx = fmaxf(keeper.rect.x, fminf(ball.pos.x, keeper.rect.x + keeper.rect.width));
        float ny = fmaxf(keeper.rect.y, fminf(ball.pos.y, keeper.rect.y + keeper.rect.height));
        float dx = ball.pos.x - nx;
        float dy = ball.pos.y - ny;

        if (dx * dx + dy * dy <= ball.radius * ball.radius) { // Va chạm thủ môn
            ball.moving = false;
            shotFailed = true;
            ball.Reset();
            shotState = STATE_READY;
            nextShotReady = true;
        }

        // ==========================================================
        // *** SỬA LỖI SÚT CHÉO (BƯỚC 4): Thêm điều kiện vào 'if' ***
        //
        if (!goal_scored && !shotIsOverBar && !shotIsOutWide &&
            ball.pos.y >= goal.y &&
            ball.pos.y <= goal.y + goal.height &&
            ball.pos.x >= goal.x &&
            ball.pos.x <= goal.x + goal.width)
        {
            goalsCount++;
            goal_scored = true;
            ball.moving = false;
            ball.Reset();
            shotState = STATE_READY;
            nextShotReady = true;
        }
        // ==========================================================

        if (ball.pos.y < -100 || ball.pos.x < -100 || ball.pos.x > SCREEN_W + 100) { // Bóng bay ra ngoài
            ball.moving = false;
            shotFailed = true;
            ball.Reset();
            shotState = STATE_READY;
            nextShotReady = true;
        }
    }

    if (shots >= MAX_SHOTS && !ball.moving && nextShotReady) {
        state = STATE_RESULT;
    }
}

void Game::UpdateResult() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mouse, btnPlayAgain.rect)) {
            state = STATE_GAME;
            ResetGame();
        }
        else if (CheckCollisionPointRec(mouse, btnMenu.rect)) {
            state = STATE_MENU;
        }
    }
}

// === CÁC HÀM DRAW CHI TIẾT ===

void Game::DrawMenu() {
    ClearBackground(DARKBLUE);
    DrawText("PENALTY KICK 2D", SCREEN_W / 2 - 200, 150, 40, YELLOW);
    DrawButton(btnStart);
    DrawButton(btnInstr);
    DrawButton(btnSound);
    DrawButton(btnExit);
}

void Game::DrawInstructions() {
    ClearBackground(BLACK);
    DrawText("Instructions (New Logic):", 200, 120, 30, YELLOW);
    DrawText("- HOLD 'D': To aim UP (set elevation)", 100, 180, 20, WHITE);
    DrawText("- RELEASE 'D': To lock elevation", 100, 210, 20, WHITE);
    DrawText("- You have 1 SECOND to aim Left/Right (Arrow Keys)", 100, 240, 20, WHITE);
    DrawText("- The ball will shoot AUTOMATICALLY!", 100, 270, 20, LIME);
    DrawText("- WARNING: Aiming too high (above goal) will miss!", 100, 300, 20, RED);
    DrawText("Click to return...", 200, 450, 20, GRAY);
}

void Game::DrawGame() {
    ClearBackground({ 50,140,50,255 });

    if (hasGrass)
        DrawTexturePro(texGrass, { 0,0,(float)texGrass.width,(float)texGrass.height }, { 0,0,(float)SCREEN_W,(float)SCREEN_H }, { 0,0 }, 0, WHITE);

    if (hasGoal) {
        // (Đây là logic vẽ của bạn, tôi giữ nguyên)
        float visualX = goal.x - 20.0f;
        float visualY = goal.y - 20.0f;
        float visualW = 400.0f;
        float visualH = 270.0f;
        DrawTexturePro(texGoal, { 0,0,(float)texGoal.width,(float)texGoal.height }, { visualX, visualY, visualW, visualH }, { 0,0 }, 0, WHITE);
    }
    else {
        DrawRectangleLinesEx(goal, 3, WHITE);
    }

    if (hasKeeper)
        DrawTexturePro(texKeeper, { 0,0,(float)texKeeper.width,(float)texKeeper.height }, { keeper.rect.x,keeper.rect.y,keeper.rect.width,keeper.rect.height }, { 0,0 }, 0, WHITE);
    else
        DrawRectangleRec(keeper.rect, BLUE);


    if (shotState == STATE_AIM_VERTICAL || shotState == STATE_AIM_HORIZONTAL) {
        Vector2 drawPos = aimReticle;
        DrawCircleLines(drawPos.x, drawPos.y, aimRadius, Fade(RED, 0.7f));
        DrawLine(drawPos.x - 5, drawPos.y, drawPos.x + 5, drawPos.y, RED);
        DrawLine(drawPos.x, drawPos.y - 5, drawPos.x, drawPos.y + 5, RED);
    }


    if (hasBall)
        DrawTexturePro(texBall, { 0,0,(float)texBall.width,(float)texBall.height }, { ball.pos.x - ball.radius,ball.pos.y - ball.radius,ball.radius * 3,ball.radius * 3 }, { 0,0 }, 0, WHITE);
    else
        DrawCircleV(ball.pos, ball.radius, ORANGE);

    // HUD
    char hud[128];
    sprintf_s(hud, "Shots: %d/7   Goals: %d", shots, goalsCount);
    DrawText(hud, 20, 20, 24, WHITE);

    // HUD Hướng dẫn
    if (shotState == STATE_READY && nextShotReady) {
        DrawText("PRESS 'D' to set elevation", SCREEN_W / 2 - 130, SCREEN_H - 50, 20, WHITE);
    }
    else if (shotState == STATE_AIM_VERTICAL) {
        DrawText("HOLD 'D' to aim up... RELEASE to lock", SCREEN_W / 2 - 180, SCREEN_H - 50, 20, YELLOW);
    }
    else if (shotState == STATE_AIM_HORIZONTAL) {
        char hudHint[128];
        sprintf_s(hudHint, "Use ARROWS to aim! Shooting in: %.1f", aimTimer);
        DrawText(hudHint, SCREEN_W / 2 - 190, SCREEN_H - 50, 20, LIME);
    }
}

void Game::DrawResult() {
    ClearBackground(BLACK);
    DrawText("RESULT", SCREEN_W / 2 - 120, 100, 40, YELLOW);

    char res[128];
    sprintf_s(res, "Goals: %d/7", goalsCount);
    DrawText(res, SCREEN_W / 2 - 100, 200, 30, WHITE);

    DrawButton(btnPlayAgain);
    DrawButton(btnMenu);
}
