#ifndef DEFS_H
#define DEFS_H

// File used to define game configuration.

// Define size of window
constexpr int SCREEN_WIDTH  = 1366;
constexpr int SCREEN_HEIGHT = 768;

// Define how long one frame should be
// 16 ms is equivalent to ~60 FPS (really 62.5 FPS)
constexpr int DELTA_TIME = 16; // ms

// Define all possible game states
enum GameState {
    MAIN_MENU = 0,
    GAME_MODE_1 = 1,
    SETTINGS = 2
};

// Bounding box definition
struct BoundingBox {
  BoundingBox(int x, int y, int w, int h)
      : left(x), right(x + w), top(y), bottom(y + h) {}
  int left;
  int right;
  int top;
  int bottom;
};

#endif // DEFS_H