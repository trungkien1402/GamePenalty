#pragma once
#include "raylib.h"
#include "constants.h"
#include "ball.h"
#include "keeper.h"
#include "button.h"
#include "GameState.h"

class Game {
public:
    Game();
    ~Game();

    void Update(GameState& globalState);
    void Draw();
    void InitGame();
    void UnloadGame();

private:
    void UpdatePlaying(GameState& globalState);
    void UpdateResult(GameState& globalState);
    void DrawPlaying();
    void DrawResult();

    InternalState internalState;
    Vector2 mouse;
    float dt;

    Texture2D texGoal, texKeeper, texBall, texGrass;
    bool hasGoal, hasKeeper, hasBall, hasGrass;

    Button btnPlayAgain, btnBackToMenu;
    Rectangle goal;
    Ball ball;
    Keeper keeper;

    int shots;
    int goalsCount;
    bool nextShotReady;
    bool goal_scored;
    bool shotFailed;
    bool shotIsOverBar;
    bool shotIsOutWide;

    ShotState shotState;
    Vector2 aimStartPos;
    Vector2 aimReticle;
    float aimRadius;
    float aimTimer;

    const float AIM_SPEED_Y = 400.0f;
    const float AIM_SPEED_X = 400.0f;
    const float POWER_BASE = 950.0f;
    const float AIM_TIMER_DURATION = 1.0f;
};
