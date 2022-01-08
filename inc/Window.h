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
  inline void EnemyMoves(int enemyIdx) {
    EXECUTE_IF_VALID(m_enemies[enemyIdx], m_enemies[enemyIdx]->Move());
  }
  inline void PlayerFires() { EXECUTE_IF_VALID(m_player, m_player->Fire()); }
  inline void EnemyFires(int enemyIdx) {
    EXECUTE_IF_VALID(m_enemies[enemyIdx], m_enemies[enemyIdx]->Fire());
  }

  void CollisionDetection(int enemyIdx);

  // Start with 3 for now
  int m_startingNumEnemies = 10;

private:
  SDL_Window *m_window = NULL;
  SDL_Surface *m_screenSurface = NULL;
  SDL_Renderer *m_renderer = NULL;

  std::unique_ptr<MainMenu> m_mainMenu = nullptr;
  std::unique_ptr<Player> m_player = nullptr;
  std::vector<std::unique_ptr<Enemy>> m_enemies;

  int m_numEnemies = m_startingNumEnemies;
};

#endif // WINDOW_H
