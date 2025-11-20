#pragma once
#include "raylib.h"
#include "constants.h"
#include "ball.h"
#include "keeper.h"
#include "button.h"
#include "GameState.h"
#include "sound.h"

class Game {
public:
    Game();
    ~Game();

    void Update(GameState& globalState, SoundManager& soundManager);
    void Draw();
    void InitGame();
    void UnloadGame();

private:
    void UpdatePlaying(GameState& globalState, SoundManager& soundManager);
    void UpdateResult(GameState& globalState);
    void DrawPlaying();
    void DrawResult();

    InternalState internalState;
    Vector2 mouse;
    float dt;

    // --- ASSETS ---
    Texture2D texGoal, texKeeper, texBall, texGrass;
    Texture2D texKeeperLeft, texKeeperRight;

    bool hasGoal, hasKeeper, hasBall, hasGrass;
    bool hasKeeperLeft, hasKeeperRight;

    Button btnPlayAgain, btnBackToMenu;
    Rectangle goal;
    Ball ball;
    Keeper keeper;

    int shots;
    int goalsCount;
    bool nextShotReady;

    // Biến kết quả
    bool goal_scored;
    bool shotFailed;
    bool shotIsOverBar;
    bool shotIsOutWide;

    // Biến để hiện chữ GOAL/MISS
    bool showResult;     // Có đang hiện chữ không?
    float resultTimer;   // Thời gian hiện chữ (2 giây)

    ShotState shotState;
    Vector2 aimStartPos;
    Vector2 aimReticle;
    float aimRadius;
    float aimTimer;

    const float AIM_SPEED_Y = 870.0f;
    const float AIM_SPEED_X = 700.0f;
    const float POWER_BASE = 950.0f;
    const float AIM_TIMER_DURATION = 1.0f;
};
