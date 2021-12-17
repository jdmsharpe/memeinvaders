#ifndef WINDOW_H
#define WINDOW_H

#include "Player.h"
#include "Enemy.h"

// Owns all in-game entities and is in charge of rendering
// and displaying said entities to screen.
class Window {
public:
  Window();
  ~Window(){};

  // Create window, surface, and renderer
  bool Open();
  // Destroy window, surface, and renderer
  void Close();
  // Update entity positions and process commands
  void Render(const GameState &gameState);

  inline void PlayerMoves(const Uint8 *keyboardState) {
    m_player->Move(keyboardState);
  }
  inline void EnemyMoves() {
    m_enemy->Move();
  }
  inline void PlayerFires() {
    m_player->Fire();
  }

private:
  // The window we'll be rendering to
  SDL_Window *m_window = NULL;

  // The surface contained by the window
  SDL_Surface *m_screenSurface = NULL;

  // The renderer
  SDL_Renderer *m_renderer = NULL;

  // The player
  Player *m_player = nullptr;

  // The player
  Enemy *m_enemy = nullptr;
};

#endif // WINDOW_H
