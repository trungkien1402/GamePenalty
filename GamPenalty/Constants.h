#pragma once

// Quy hoạch về kích thước của bên Menu (rộng hơn)
#define SCREEN_W 1500 
#define SCREEN_H 900

#define MAX_SHOTS 7
#define PI 3.1415926f

// Trạng thái nội bộ của Gameplay
enum InternalState {
    INTERNAL_PLAYING,
    INTERNAL_RESULT
};

enum ShotState { STATE_READY, STATE_AIM_VERTICAL, STATE_AIM_HORIZONTAL, STATE_FIRED };
