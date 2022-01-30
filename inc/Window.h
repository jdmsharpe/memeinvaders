#ifndef WINDOW_H
#define WINDOW_H

#include "Enemy.h"
#include "HighScore.h"
#include "MainMenu.h"
#include "MainMenuOptions.h"
#include "Player.h"
#include "Score.h"

// Owns all in-game entities and is in charge of rendering
// and displaying said entities to screen.
class Window {
public:
  Window();
  ~Window(){};

  // Create window, surface, and renderer
  bool Open();
  // Create all game entities
  void CreateEntities();
  // If player presses reset...
  void ResetGameMode1(bool resetScore, int difficulty);
  // Destroy window, surface, and renderer
  void Close();
  // Update entity positions and process commands
  void Render(const GameState &gameState);
  // Make game harder on level increment
  void IncreaseDifficulty();

  // Functions to update player and enemies from inputs
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

  // Check if projectiles have hit target
  void CollisionDetection(int enemyIdx, std::pair<bool, bool> &collisionResult);

  // Process results of collision detection
  void ProcessEvents(int enemyIdx,
                     const std::pair<bool, bool> &collisionResult);

  // Start with 14 for now
  int m_startingNumEnemies = 14;

private:
  SDL_Window *m_window = NULL;
  SDL_Surface *m_screenSurface = NULL;
  SDL_Renderer *m_renderer = NULL;

  std::unique_ptr<MainMenu> m_mainMenu = nullptr;
  std::unique_ptr<MainMenuOptions> m_mainMenuOptions = nullptr;
  std::unique_ptr<Player> m_player = nullptr;
  std::vector<std::unique_ptr<Enemy>> m_enemies;
  std::unique_ptr<Score> m_score = nullptr;
  std::unique_ptr<HighScore> m_highScore = nullptr;

  int m_numEnemies = m_startingNumEnemies;

  // Help make sure there is no weird 1-up behavior
  bool m_validCheckpoint = false;
  // Track if game has ended
  bool m_gameOver = false;
  // Track if it's time to increment level
  bool m_nextLevel = false;
  // Current level (determines difficulty)
  int m_level = 1;
};

#endif // WINDOW_H
