#include "game.h"
#include <cstdio>  // Dùng cho sprintf, TextFormat
#include <cmath>   // Dùng cho fmaxf, fminf, cos, sin

// === Constructor ===
Game::Game() {
    texGoal = { 0 }; texKeeper = { 0 }; texBall = { 0 }; texGrass = { 0 };
    texKeeperLeft = { 0 }; texKeeperRight = { 0 };
    internalState = INTERNAL_PLAYING;

    // [QUAN TRỌNG] Khởi tạo giá trị mặc định để không bị lỗi hiển thị
    showResult = false;
    goal_scored = false;
    shotFailed = false;
    resultTimer = 0.0f;
}

// === Destructor ===
Game::~Game() {
    UnloadGame();
}

// === Khởi tạo Game ===
void Game::InitGame() {
    // 1. Load Assets
    const char* ASSET_GOAL = "Assets/gon.png";
    const char* ASSET_KEEPER = "Assets/keeper.png";
    const char* ASSET_KEEPER_LEFT = "Assets/keeper_left.png";
    const char* ASSET_KEEPER_RIGHT = "Assets/keeper_right.png";
    const char* ASSET_BALL = "Assets/ball.png";
    const char* ASSET_GRASS = "Assets/grass.png";

    hasGoal = FileExists(ASSET_GOAL);
    hasKeeper = FileExists(ASSET_KEEPER);
    hasKeeperLeft = FileExists(ASSET_KEEPER_LEFT);
    hasKeeperRight = FileExists(ASSET_KEEPER_RIGHT);
    hasBall = FileExists(ASSET_BALL);
    hasGrass = FileExists(ASSET_GRASS);

    if (hasGoal && texGoal.id == 0) texGoal = LoadTexture(ASSET_GOAL);
    if (hasKeeper && texKeeper.id == 0) texKeeper = LoadTexture(ASSET_KEEPER);
    if (hasKeeperLeft && texKeeperLeft.id == 0) texKeeperLeft = LoadTexture(ASSET_KEEPER_LEFT);
    if (hasKeeperRight && texKeeperRight.id == 0) texKeeperRight = LoadTexture(ASSET_KEEPER_RIGHT);
    if (hasBall && texBall.id == 0) texBall = LoadTexture(ASSET_BALL);
    if (hasGrass && texGrass.id == 0) texGrass = LoadTexture(ASSET_GRASS);

    // Setup Buttons
    float BTN_W = 260; float BTN_H = 55;
    float centerX = (SCREEN_W - BTN_W) / 2.0f;
    btnPlayAgain = { { centerX, 600, BTN_W, BTN_H }, "Play Again", DARKGREEN };
    btnBackToMenu = { { centerX, 670, BTN_W, BTN_H }, "Back to Menu", DARKBLUE };

    // GÔN 850x350
    float goalWidth = 850.0f;
    float goalHeight = 350.0f;
    float goalX = (SCREEN_W - goalWidth) / 2.0f;
    float goalY = 130.0f;

    goal = { goalX, goalY, goalWidth, goalHeight };

    // Reset thủ môn
    keeper.Reset(goal.x, goal.width, goal.y, goal.height - 30.0f);

    aimStartPos = { SCREEN_W / 2.0f, goal.y + goal.height + 40.0f };

    // Reset Logic Vars
    shots = 0; goalsCount = 0;
    ball.Reset();
    aimReticle = aimStartPos;
    shotState = STATE_READY;
    nextShotReady = true;
    aimTimer = 0.0f;

    goal_scored = false; shotFailed = false; shotIsOverBar = false; shotIsOutWide = false;

    // Reset bộ đếm hiện chữ
    showResult = false;
    resultTimer = 0.0f;

    internalState = INTERNAL_PLAYING;
}

// === Giải phóng ===
void Game::UnloadGame() {
    if (texGoal.id != 0) UnloadTexture(texGoal);
    if (texKeeper.id != 0) UnloadTexture(texKeeper);
    if (texKeeperLeft.id != 0) UnloadTexture(texKeeperLeft);
    if (texKeeperRight.id != 0) UnloadTexture(texKeeperRight);
    if (texBall.id != 0) UnloadTexture(texBall);
    if (texGrass.id != 0) UnloadTexture(texGrass);

    texGoal.id = 0; texKeeper.id = 0;
    texKeeperLeft.id = 0; texKeeperRight.id = 0;
    texBall.id = 0; texGrass.id = 0;
}

// === Update Chung ===
void Game::Update(GameState& globalState) {
    dt = GetFrameTime();
    mouse = GetMousePosition();
    if (IsKeyPressed(KEY_ESCAPE)) globalState = STATE_MENU;

    if (internalState == INTERNAL_PLAYING) UpdatePlaying(globalState);
    else if (internalState == INTERNAL_RESULT) UpdateResult(globalState);
}

// === Update Logic Gameplay ===
void Game::UpdatePlaying(GameState& globalState) {

    // --- LOGIC HIỆN CHỮ KẾT QUẢ (Delay 2 giây) ---
    if (showResult) {
        resultTimer -= dt;
        if (resultTimer <= 0.0f) {
            showResult = false;
            ball.Reset();
            shotState = STATE_READY;
            nextShotReady = true;

            if (shots >= MAX_SHOTS) {
                internalState = INTERNAL_RESULT;
            }
        }
        return; // Dừng xử lý sút khi đang hiện chữ
    }

    // 1. Chuẩn bị
    if (shotState == STATE_READY && nextShotReady) {
        aimReticle = aimStartPos;
        if (IsKeyPressed(KEY_D)) {
            shotState = STATE_AIM_VERTICAL;
            nextShotReady = false;
            goal_scored = false; shotFailed = false; shotIsOverBar = false; shotIsOutWide = false;
            ball.Reset();
        }
    }

    // 2. Chỉnh độ cao
    if (shotState == STATE_AIM_VERTICAL) {
        if (IsKeyDown(KEY_D)) {
            aimReticle.y -= AIM_SPEED_Y * dt;
            aimReticle.y = fmaxf(goal.y - 200, aimReticle.y);
        }
        if (IsKeyReleased(KEY_D)) {
            shotState = STATE_AIM_HORIZONTAL;
            aimReticle.x = aimStartPos.x;
            aimTimer = AIM_TIMER_DURATION;
        }
    }

    // 3. Chỉnh hướng ngang
    if (shotState == STATE_AIM_HORIZONTAL) {
        aimTimer -= dt;
        if (IsKeyDown(KEY_LEFT)) aimReticle.x -= AIM_SPEED_X * dt;
        if (IsKeyDown(KEY_RIGHT)) aimReticle.x += AIM_SPEED_X * dt;

        float clampPadding = 300.0f;
        aimReticle.x = fmaxf(goal.x - clampPadding, fminf(aimReticle.x, goal.x + goal.width + clampPadding));
        aimRadius = 30.0f;

        if (aimTimer <= 0.0f) shotState = STATE_FIRED;
    }

    // 4. Giai đoạn SÚT
    if (shotState == STATE_FIRED && !ball.moving) {
        ball.moving = true;
        shots++;
        keeper.dir = GetRandomValue(0, 2) - 1;

        Vector2 target = aimReticle;

        float crossbarThickness = 35.0f;
        shotIsOverBar = (aimReticle.y < goal.y + crossbarThickness);

        float postMargin = 25.0f;
        if (aimReticle.x > (goal.x + postMargin) &&
            aimReticle.x < (goal.x + goal.width - postMargin)) {
            shotIsOutWide = false;
        }
        else {
            shotIsOutWide = true;
        }

        if (shotIsOverBar) target = { aimReticle.x + GetRandomValue(-50, 50), goal.y - 450 };

        float angle = atan2f(target.y - ball.pos.y, target.x - ball.pos.x);
        ball.vel.x = cosf(angle) * POWER_BASE;
        ball.vel.y = sinf(angle) * POWER_BASE;
    }

    // Update Thủ môn & Bóng
    keeper.Update(dt, ball.moving);

    if (ball.moving) {
        ball.pos.x += ball.vel.x * dt;
        ball.pos.y += ball.vel.y * dt;

        // 5A. Va chạm Thủ môn
        float nx = fmaxf(keeper.rect.x, fminf(ball.pos.x, keeper.rect.x + keeper.rect.width));
        float ny = fmaxf(keeper.rect.y, fminf(ball.pos.y, keeper.rect.y + keeper.rect.height));
        float dx = ball.pos.x - nx;
        float dy = ball.pos.y - ny;

        if (dx * dx + dy * dy <= ball.radius * ball.radius) {
            ball.moving = false;
            shotFailed = true;

            // Bật thông báo
            showResult = true;
            resultTimer = 2.0f;
            return;
        }

        // 5B. Kiểm tra Ghi Bàn (Bóng dừng tại tâm ngắm)
        float stopLine = aimReticle.y;

        if (!goal_scored && !shotFailed && ball.pos.y <= stopLine) {
            if (shotIsOverBar || shotIsOutWide) {
                // Out - Để bóng bay tiếp
            }
            else {
                // Goal
                goalsCount++;
                goal_scored = true;
                ball.moving = false;

                // Bật thông báo
                showResult = true;
                resultTimer = 2.0f;
                return;
            }
        }

        // 5C. Reset ra ngoài
        if (ball.pos.y < -200 || ball.pos.x < -200 || ball.pos.x > SCREEN_W + 200) {
            ball.moving = false;
            shotFailed = true;

            // Bật thông báo
            showResult = true;
            resultTimer = 2.0f;
            return;
        }
    }
}

// === Update Kết quả ===
void Game::UpdateResult(GameState& globalState) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mouse, btnPlayAgain.rect)) InitGame();
        else if (CheckCollisionPointRec(mouse, btnBackToMenu.rect)) globalState = STATE_MENU;
    }
}

// === Vẽ Game ===
void Game::Draw() {
    if (internalState == INTERNAL_PLAYING) DrawPlaying();
    else if (internalState == INTERNAL_RESULT) DrawResult();
}

void Game::DrawPlaying() {
    ClearBackground({ 50,140,50,255 });

    if (hasGrass) DrawTexturePro(texGrass, { 0,0,(float)texGrass.width,(float)texGrass.height }, { 0,0,(float)SCREEN_W,(float)SCREEN_H }, { 0,0 }, 0, WHITE);

    if (hasGoal) {
        Rectangle destRect = { goal.x - 30, goal.y - 30, goal.width + 60, goal.height + 40 };
        DrawTexturePro(texGoal, { 0,0,(float)texGoal.width,(float)texGoal.height }, destRect, { 0,0 }, 0, WHITE);
    }
    else DrawRectangleLinesEx(goal, 3, WHITE);

    // VẼ THỦ MÔN (3 ẢNH)
    Texture2D textureToDraw = texKeeper;
    bool useTexture = hasKeeper;

    if (ball.moving) {
        if (keeper.dir == -1 && hasKeeperLeft) { textureToDraw = texKeeperLeft; useTexture = true; }
        else if (keeper.dir == 1 && hasKeeperRight) { textureToDraw = texKeeperRight; useTexture = true; }
    }
    if (useTexture) DrawTexturePro(textureToDraw, { 0, 0, (float)textureToDraw.width, (float)textureToDraw.height }, keeper.rect, { 0, 0 }, 0, WHITE);
    else DrawRectangleRec(keeper.rect, BLUE);

    // VẼ TÂM NGẮM (Ẩn khi hiện kết quả)
    if (!showResult && (shotState == STATE_AIM_VERTICAL || shotState == STATE_AIM_HORIZONTAL)) {
        DrawRing(aimReticle, aimRadius - 4, aimRadius, 0, 360, 36, RED);
        DrawLineEx({ aimReticle.x - 20, aimReticle.y }, { aimReticle.x + 20, aimReticle.y }, 4.0f, RED);
        DrawLineEx({ aimReticle.x, aimReticle.y - 20 }, { aimReticle.x, aimReticle.y + 20 }, 4.0f, RED);
    }

    // VẼ BÓNG
    if (hasBall) DrawTexturePro(texBall, { 0,0,(float)texBall.width,(float)texBall.height }, { ball.pos.x - ball.radius,ball.pos.y - ball.radius,ball.radius * 3,ball.radius * 3 }, { 0,0 }, 0, WHITE);
    else DrawCircleV(ball.pos, ball.radius, ORANGE);

    // HUD
    DrawText(TextFormat("Shots: %d/%d", shots, MAX_SHOTS), 20, 20, 30, WHITE);
    DrawText(TextFormat("Goals: %d", goalsCount), 20, 60, 30, YELLOW);

    if (shotState == STATE_READY && nextShotReady && !showResult) DrawText("PRESS 'D' to start aiming height", SCREEN_W / 2 - 180, SCREEN_H - 50, 24, WHITE);
    else if (shotState == STATE_AIM_VERTICAL) DrawText("HOLD 'D' to aim UP... RELEASE to lock", SCREEN_W / 2 - 220, SCREEN_H - 50, 24, YELLOW);
    else if (shotState == STATE_AIM_HORIZONTAL) DrawText(TextFormat("Use ARROWS Left/Right! Shoot in: %.1f", aimTimer), SCREEN_W / 2 - 230, SCREEN_H - 50, 24, LIME);

    // ========================================================
    // *** [CĂN GIỮA CHUẨN] VẼ CHỮ GOAL / MISS ***
    // ========================================================
    if (showResult) {
        const char* text;
        Color color;

        if (goal_scored) {
            text = "GOAL!!!";
            color = GREEN;
        }
        else {
            text = "MISS!";
            color = RED;
        }

        int fontSize = 150;
        int textW = MeasureText(text, fontSize);

        // Tính toán tọa độ X để căn giữa màn hình
        int textX = (SCREEN_W - textW) / 2;
        int textY = (SCREEN_H - fontSize) / 2 - 50;

        DrawText(text, textX + 5, textY + 5, fontSize, BLACK); // Bóng đổ
        DrawText(text, textX, textY, fontSize, color);         // Chữ chính
    }
}

void Game::DrawResult() {
    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, Fade(BLACK, 0.85f));

    // ========================================================
    // *** [CĂN GIỮA CHUẨN] MÀN HÌNH KẾT QUẢ ***
    // ========================================================

    const char* title = "MATCH RESULT";
    int titleSize = 60;
    int titleW = MeasureText(title, titleSize);
    DrawText(title, (SCREEN_W - titleW) / 2, 150, titleSize, YELLOW);

    const char* msg = (goalsCount >= 4) ? "GREAT JOB!" : "TRY HARDER!";
    Color msgColor = (goalsCount >= 4) ? GREEN : RED;
    int msgSize = 50;
    int msgW = MeasureText(msg, msgSize);
    DrawText(msg, (SCREEN_W - msgW) / 2, 250, msgSize, msgColor);

    const char* scoreText = TextFormat("Goals Scored: %d / %d", goalsCount, MAX_SHOTS);
    int scoreSize = 40;
    int scoreW = MeasureText(scoreText, scoreSize);
    DrawText(scoreText, (SCREEN_W - scoreW) / 2, 330, scoreSize, WHITE);

    DrawButton(btnPlayAgain);
    DrawButton(btnBackToMenu);
}
