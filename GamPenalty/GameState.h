#pragma once
// File này chứa enum trạng thái game và được dùng chung giữa Menu và Game

// Liệt kê các trạng thái chính của trò chơi
enum GameState {
    STATE_MENU,          // Trạng thái menu chính
    STATE_INSTRUCTIONS,  // Trạng thái xem hướng dẫn
    STATE_SOUND,         // Trạng thái chỉnh âm thanh
    STATE_GAME,          // Trạng thái đang chơi game
    STATE_EXIT           // Thoát game
};
