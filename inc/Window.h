#ifndef WINDOW_H
#define WINDOW_H

#include "Enemy.h"
#include "MainMenu.h"
#include "Player.h"
#include <chrono>
#include <thread>

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
  inline void EnemyFires() {
    m_enemy->Fire();
    // std::chrono::milliseconds timespan(1000); // or whatever
    // std::this_thread::sleep_for(timespan);
  }

private:
  SDL_Window *m_window = NULL;
  SDL_Surface *m_screenSurface = NULL;
  SDL_Renderer *m_renderer = NULL;

  std::unique_ptr<Player> m_player = nullptr;
  std::unique_ptr<Enemy> m_enemy = nullptr;
  std::unique_ptr<MainMenu> m_mainMenu = nullptr;
};

#endif // WINDOW_H
