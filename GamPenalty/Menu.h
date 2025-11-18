#ifndef MENU_H
#define MENU_H

#include "raylib.h"

enum GameState {
    STATE_MENU,
    STATE_GAME,
    STATE_INSTRUCTIONS,
    STATE_SOUND,
    STATE_EXIT
};

// Hàm menu, trả về state mới
GameState UpdateMenu(GameState currentState);

#endif
