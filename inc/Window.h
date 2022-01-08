#ifndef WINDOW_H
#define WINDOW_H

#include "Enemy.h"
#include "MainMenu.h"
#include "Player.h"

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
    EXECUTE_IF_VALID(m_player, m_player->Move(keyboardState));
  }
  inline void EnemyMoves() {
    EXECUTE_IF_VALID(m_enemy, m_enemy->Move());
  }
  inline void PlayerFires() {
    EXECUTE_IF_VALID(m_player, m_player->Fire());
  }
  inline void EnemyFires() {
    EXECUTE_IF_VALID(m_enemy, m_enemy->Fire());
  }

  void CollisionDetection();

private:
  SDL_Window *m_window = NULL;
  SDL_Surface *m_screenSurface = NULL;
  SDL_Renderer *m_renderer = NULL;

  std::unique_ptr<Player> m_player = nullptr;
  std::unique_ptr<Enemy> m_enemy = nullptr;
  std::unique_ptr<MainMenu> m_mainMenu = nullptr;
};

#endif // WINDOW_H
