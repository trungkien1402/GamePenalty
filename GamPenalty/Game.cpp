#include "game.h"
#include <cstdio>  // Dùng cho sprintf, TextFormat
#include <cmath>   // Dùng cho fmaxf, fminf, cos, sin, sqrtf

// === Constructor ===
Game::Game() {
    // Khởi tạo các Texture2D
    texGoal = { 0 }; texKeeper = { 0 }; texBall = { 0 }; texGrass = { 0 };
    texKeeperLeft = { 0 }; texKeeperRight = { 0 };

    // [FIX C26495] Khởi tạo tất cả các biến
    internalState = INTERNAL_PLAYING;
    dt = 0.0f;

    // Biến Assets
    hasGoal = hasKeeper = hasBall = hasGrass = false;
    hasKeeperLeft = hasKeeperRight = false;

    // Khởi tạo các biến Button/Rectangle
    btnPlayAgain = { {0,0,0,0}, "", BLACK };
    btnBackToMenu = { {0,0,0,0}, "", BLACK };
    goal = { 0, 0, 0, 0 }; // Khởi tạo Rectangle

    // Biến logic
    shots = 0; goalsCount = 0;
    nextShotReady = true;

    // Biến kết quả
    showResult = false;
    goal_scored = false;
    shotFailed = false;
    shotIsOverBar = false;
    shotIsOutWide = false;
    resultTimer = 0.0f;

    // Biến aim
    shotState = STATE_READY;
    aimRadius = 30.0f;
    aimTimer = 0.0f;
    aimStartPos = { 0.0f, 0.0f };
    aimReticle = { 0.0f, 0.0f };
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

    // Đặt tâm ngắm
    aimStartPos = { SCREEN_W / 2.0f, goal.y + goal.height - 100.0f };

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

// === Giải phóng (Giữ nguyên) ===
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

// === Update Chung (Giữ nguyên) ===
void Game::Update(GameState& globalState, SoundManager& soundManager) {
    dt = GetFrameTime();
    mouse = GetMousePosition();
    if (IsKeyPressed(KEY_ESCAPE)) globalState = STATE_MENU;

    if (internalState == INTERNAL_PLAYING) UpdatePlaying(globalState, soundManager);
    else if (internalState == INTERNAL_RESULT) UpdateResult(globalState);
}

// === Update Logic Gameplay ===
void Game::UpdatePlaying(GameState& globalState, SoundManager& soundManager) {

    // --- LOGIC HIỆN CHỮ KẾT QUẢ (Delay) ---
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

    // 1. Chuẩn bị (Giữ nguyên)
    if (shotState == STATE_READY && nextShotReady) {
        aimReticle = aimStartPos;
        if (IsKeyPressed(KEY_D)) {
            shotState = STATE_AIM_VERTICAL;
            nextShotReady = false;
            goal_scored = false; shotFailed = false; shotIsOverBar = false; shotIsOutWide = false;
            ball.Reset();
        }
    }

    // 2. Chỉnh độ cao (Giữ nguyên)
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

    // 3. Chỉnh hướng ngang (Giữ nguyên)
    if (shotState == STATE_AIM_HORIZONTAL) {
        aimTimer -= dt;
        if (IsKeyDown(KEY_LEFT)) aimReticle.x -= AIM_SPEED_X * dt;
        if (IsKeyDown(KEY_RIGHT)) aimReticle.x += AIM_SPEED_X * dt;

        float clampPadding = 300.0f;
        aimReticle.x = fmaxf(goal.x - clampPadding, fminf(aimReticle.x, goal.x + goal.width + clampPadding));
        aimRadius = 30.0f;

        if (aimTimer <= 0.0f) shotState = STATE_FIRED;
    }

    // 4. Giai đoạn SÚT (Giữ nguyên)
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

        // ====================================================
        // 5A. XỬ LÝ BÓNG ĐẬP THỦ MÔN VĂNG RA (Giữ nguyên)
        // ====================================================

        // Tìm điểm gần nhất trên hình chữ nhật thủ môn
        float closestX = fmaxf(keeper.rect.x, fminf(ball.pos.x, keeper.rect.x + keeper.rect.width));
        float closestY = fmaxf(keeper.rect.y, fminf(ball.pos.y, keeper.rect.y + keeper.rect.height));

        float dx = ball.pos.x - closestX;
        float dy = ball.pos.y - closestY;
        float distSqr = dx * dx + dy * dy;

        // Nếu va chạm (khoảng cách nhỏ hơn bán kính) VÀ chưa bị tính là sút hỏng
        if (distSqr <= (ball.radius * ball.radius) && !shotFailed) {

            shotFailed = true; // Đánh dấu là sút hỏng (Cứu thua)

            // Tính vector phản xạ
            float dist = sqrtf(distSqr);
            float normalX, normalY;

            // Tránh chia cho 0 nếu tâm bóng trùng khít điểm va chạm
            if (dist == 0.0f) { normalX = 0; normalY = 1; }
            else { normalX = dx / dist; normalY = dy / dist; }

            // Đẩy bóng ra khỏi vùng va chạm 1 chút để không bị kẹt
            ball.pos.x = closestX + normalX * (ball.radius + 2.0f);
            ball.pos.y = closestY + normalY * (ball.radius + 2.0f);

            // Tính tốc độ hiện tại
            float currentSpeed = sqrtf(ball.vel.x * ball.vel.x + ball.vel.y * ball.vel.y);

            // Giảm lực nảy (0.8 = nảy lại 80% lực)
            float bounceForce = currentSpeed * 2.0f;

            // Gán vận tốc mới theo hướng phản xạ
            ball.vel.x = normalX * bounceForce;
            ball.vel.y = normalY * bounceForce;

            // [QUAN TRỌNG]: Không gọi return hay showResult ở đây.
        }

        // 5B. Kiểm tra Ghi Bàn (Chỉ check nếu chưa va chạm thủ môn)
        float stopLine = aimReticle.y;

        if (!goal_scored && !shotFailed && ball.pos.y <= stopLine) {
            if (shotIsOverBar || shotIsOutWide) {
                // Out - Để bóng bay tiếp
            }
            else {
                // Goal
                goalsCount++;
                goal_scored = true;
                ball.moving = false; // Dừng bóng nếu vào gôn

                // PHÁT ÂM THANH GHI BÀN
                soundManager.PlayGoalSFX();

                // Bật thông báo ngay lập tức nếu vào gôn
                showResult = true;
                resultTimer = 5.0f; // GOAL 5 GIÂY
                return;
            }
        }

        // 5C. Reset khi bóng bay ra ngoài màn hình
        if (ball.pos.y < -200 || ball.pos.y > SCREEN_H + 200 || ball.pos.x < -200 || ball.pos.x > SCREEN_W + 200) {
            ball.moving = false;

            // Nếu chưa có trạng thái nào thì tính là hỏng
            if (!goal_scored && !shotFailed) shotFailed = true;

            // PHÁT ÂM THANH SÚT HỎNG
            if (!goal_scored) soundManager.PlayMissSFX();

            // Lúc này mới hiện bảng kết quả
            showResult = true;
            resultTimer = 3.0f; // MISS 3 GIÂY
            return;
        }
    }
}

// === Update Kết quả & Draw (Giữ nguyên) ===
void Game::UpdateResult(GameState& globalState) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mouse, btnPlayAgain.rect)) InitGame();
        else if (CheckCollisionPointRec(mouse, btnBackToMenu.rect)) globalState = STATE_MENU;
    }
}

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

    // VẼ CHỮ KẾT QUẢ
    if (showResult) {
        const char* text;
        Color color;

        if (goal_scored) {
            text = "GOAL!!!";
            color = GREEN;
        }
        else if (shotFailed) {
            text = "MISS!";
            color = RED;
        }
        else {
            text = "MISS!";
            color = RED;
        }

        int fontSize = 150;
        int textW = MeasureText(text, fontSize);

        int textX = (SCREEN_W - textW) / 2;
        int textY = (SCREEN_H - fontSize) / 2 - 50;

        DrawText(text, textX + 5, textY + 5, fontSize, BLACK);
        DrawText(text, textX, textY, fontSize, color);
    }
}

void Game::DrawResult() {
    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, Fade(BLACK, 0.85f));

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
