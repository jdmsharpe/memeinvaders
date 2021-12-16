#ifndef DEFS_H
#define DEFS_H

// Define size of window
constexpr int SCREEN_WIDTH  = 1366;
constexpr int SCREEN_HEIGHT = 768;

constexpr int DELTA_TIME = 16; // ms

enum GameState {
    MAIN_MENU = 0,
    GAME_MODE_1 = 1,
    SETTINGS = 2
};

#endif // DEFS_H