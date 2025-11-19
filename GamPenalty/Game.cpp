#include "game.h"
#include <cstdio>
#include <cmath>

Game::Game() {
    texGoal = { 0 }; texKeeper = { 0 }; texBall = { 0 }; texGrass = { 0 };
    internalState = INTERNAL_PLAYING;
}

Game::~Game() { UnloadGame(); }

void Game::InitGame() {
    // Load Assets tương đối
    hasGoal = FileExists("Assets/gon.png");
    hasKeeper = FileExists("Assets/keeper.png");
    hasBall = FileExists("Assets/ball.png");
    hasGrass = FileExists("Assets/grass.png");

    if (hasGoal && texGoal.id == 0) texGoal = LoadTexture("Assets/gon.png");
    if (hasKeeper && texKeeper.id == 0) texKeeper = LoadTexture("Assets/keeper.png");
    if (hasBall && texBall.id == 0) texBall = LoadTexture("Assets/ball.png");
    if (hasGrass && texGrass.id == 0) texGrass = LoadTexture("Assets/grass.png");

    // Setup Buttons
    float BTN_W = 260; float BTN_H = 55;
    float centerX = (SCREEN_W - BTN_W) / 2.0f;
    btnPlayAgain = { { centerX, 350, BTN_W, BTN_H }, "Play Again", DARKGREEN };
    btnBackToMenu = { { centerX, 420, BTN_W, BTN_H }, "Back to Menu", DARKBLUE };

    // Setup Goal
    float logicX = (SCREEN_W - 400.0f) / 2.0f + 20.0f;
    goal = { logicX, 100.0f, 360.0f, 140.0f };
    keeper.Reset(goal.x, goal.width, goal.y, goal.height);
    aimStartPos = { SCREEN_W / 2.0f, goal.y + goal.height + 30.0f };

    // Reset Vars
    shots = 0; goalsCount = 0;
    ball.Reset();
    aimReticle = aimStartPos;
    shotState = STATE_READY;
    nextShotReady = true;
    aimTimer = 0.0f;
    goal_scored = false; shotFailed = false; shotIsOverBar = false; shotIsOutWide = false;
    internalState = INTERNAL_PLAYING;
}

void Game::UnloadGame() {
    if (texGoal.id != 0) UnloadTexture(texGoal);
    if (texKeeper.id != 0) UnloadTexture(texKeeper);
    if (texBall.id != 0) UnloadTexture(texBall);
    if (texGrass.id != 0) UnloadTexture(texGrass);
    texGoal.id = 0; texKeeper.id = 0; texBall.id = 0; texGrass.id = 0;
}

void Game::Update(GameState& globalState) {
    dt = GetFrameTime();
    mouse = GetMousePosition();
    if (IsKeyPressed(KEY_ESCAPE)) globalState = STATE_MENU;
    if (internalState == INTERNAL_PLAYING) UpdatePlaying(globalState);
    else if (internalState == INTERNAL_RESULT) UpdateResult(globalState);
}

void Game::UpdatePlaying(GameState& globalState) {
    if (shotState == STATE_READY && nextShotReady) {
        aimReticle = aimStartPos;
        if (IsKeyPressed(KEY_D)) {
            shotState = STATE_AIM_VERTICAL;
            nextShotReady = false;
            goal_scored = false; shotFailed = false; shotIsOverBar = false; shotIsOutWide = false;
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
            aimTimer = AIM_TIMER_DURATION;
        }
    }

    if (shotState == STATE_AIM_HORIZONTAL) {
        aimTimer -= dt;
        if (IsKeyDown(KEY_LEFT)) aimReticle.x -= AIM_SPEED_X * dt;
        if (IsKeyDown(KEY_RIGHT)) aimReticle.x += AIM_SPEED_X * dt;
        aimReticle.x = fmaxf(goal.x - 20, fminf(aimReticle.x, goal.x + goal.width + 20));
        aimRadius = 15.0f;
        if (aimTimer <= 0.0f) shotState = STATE_FIRED;
    }

    if (shotState == STATE_FIRED && !ball.moving) {
        ball.moving = true;
        shots++;
        keeper.dir = GetRandomValue(0, 2) - 1;
        Vector2 target = aimReticle;
        shotIsOverBar = (aimReticle.y < goal.y);
        shotIsOutWide = (aimReticle.x < goal.x || aimReticle.x > goal.x + goal.width);
        if (shotIsOverBar) target = { aimReticle.x + GetRandomValue(-50, 50), goal.y - 300 };
        float angle = atan2f(target.y - ball.pos.y, target.x - ball.pos.x);
        ball.vel.x = cosf(angle) * POWER_BASE;
        ball.vel.y = sinf(angle) * POWER_BASE;
    }

    keeper.Update(dt, ball.moving);

    if (ball.moving) {
        ball.pos.x += ball.vel.x * dt;
        ball.pos.y += ball.vel.y * dt;

        // Va chạm thủ môn
        float nx = fmaxf(keeper.rect.x, fminf(ball.pos.x, keeper.rect.x + keeper.rect.width));
        float ny = fmaxf(keeper.rect.y, fminf(ball.pos.y, keeper.rect.y + keeper.rect.height));
        float dx = ball.pos.x - nx;
        float dy = ball.pos.y - ny;

        if (dx * dx + dy * dy <= ball.radius * ball.radius) {
            ball.moving = false; shotFailed = true; ball.Reset();
            shotState = STATE_READY; nextShotReady = true;
        }

        // Ghi bàn
        if (!goal_scored && !shotIsOverBar && !shotIsOutWide &&
            ball.pos.y >= goal.y && ball.pos.y <= goal.y + goal.height &&
            ball.pos.x >= goal.x && ball.pos.x <= goal.x + goal.width) {
            goalsCount++; goal_scored = true; ball.moving = false; ball.Reset();
            shotState = STATE_READY; nextShotReady = true;
        }

        // Ra ngoài
        if (ball.pos.y < -100 || ball.pos.x < -100 || ball.pos.x > SCREEN_W + 100) {
            ball.moving = false; shotFailed = true; ball.Reset();
            shotState = STATE_READY; nextShotReady = true;
        }
    }
    if (shots >= MAX_SHOTS && !ball.moving && nextShotReady) internalState = INTERNAL_RESULT;
}

void Game::UpdateResult(GameState& globalState) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mouse, btnPlayAgain.rect)) InitGame();
        else if (CheckCollisionPointRec(mouse, btnBackToMenu.rect)) globalState = STATE_MENU;
    }
}

void Game::Draw() {
    if (internalState == INTERNAL_PLAYING) DrawPlaying();
    else DrawResult();
}

void Game::DrawPlaying() {
    ClearBackground({ 50,140,50,255 });
    if (hasGrass) DrawTexturePro(texGrass, { 0,0,(float)texGrass.width,(float)texGrass.height }, { 0,0,(float)SCREEN_W,(float)SCREEN_H }, { 0,0 }, 0, WHITE);

    if (hasGoal) {
        DrawTexturePro(texGoal, { 0,0,(float)texGoal.width,(float)texGoal.height }, { goal.x - 20, goal.y - 20, 400, 270 }, { 0,0 }, 0, WHITE);
    }
    else DrawRectangleLinesEx(goal, 3, WHITE);

    if (hasKeeper) DrawTexturePro(texKeeper, { 0,0,(float)texKeeper.width,(float)texKeeper.height }, keeper.rect, { 0,0 }, 0, WHITE);
    else DrawRectangleRec(keeper.rect, BLUE);

    if (shotState == STATE_AIM_VERTICAL || shotState == STATE_AIM_HORIZONTAL) {
        DrawCircleLines(aimReticle.x, aimReticle.y, aimRadius, Fade(RED, 0.7f));
        DrawLine(aimReticle.x - 5, aimReticle.y, aimReticle.x + 5, aimReticle.y, RED);
        DrawLine(aimReticle.x, aimReticle.y - 5, aimReticle.x, aimReticle.y + 5, RED);
    }

    if (hasBall) DrawTexturePro(texBall, { 0,0,(float)texBall.width,(float)texBall.height }, { ball.pos.x - ball.radius,ball.pos.y - ball.radius,ball.radius * 3,ball.radius * 3 }, { 0,0 }, 0, WHITE);
    else DrawCircleV(ball.pos, ball.radius, ORANGE);

    DrawText(TextFormat("Shots: %d/%d    Goals: %d", shots, MAX_SHOTS, goalsCount), 20, 20, 24, WHITE);

    if (shotState == STATE_READY && nextShotReady) DrawText("PRESS 'D' to set elevation", SCREEN_W / 2 - 130, SCREEN_H - 50, 20, WHITE);
    else if (shotState == STATE_AIM_VERTICAL) DrawText("HOLD 'D' to aim up... RELEASE to lock", SCREEN_W / 2 - 180, SCREEN_H - 50, 20, YELLOW);
    else if (shotState == STATE_AIM_HORIZONTAL) DrawText(TextFormat("Aim in: %.1f", aimTimer), SCREEN_W / 2 - 50, SCREEN_H - 50, 20, LIME);
}

void Game::DrawResult() {
    DrawRectangle(0, 0, SCREEN_W, SCREEN_H, Fade(BLACK, 0.8f));
    DrawText("MATCH RESULT", SCREEN_W / 2 - 150, 100, 40, YELLOW);
    DrawText(TextFormat("Goals: %d/%d", goalsCount, MAX_SHOTS), SCREEN_W / 2 - 100, 200, 30, WHITE);
    DrawButton(btnPlayAgain);
    DrawButton(btnBackToMenu);
}
