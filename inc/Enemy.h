#ifndef ENEMY_H
#define ENEMY_H

#include <chrono>
#include <deque>
#include <iostream>
#include <vector>

#include "DynamicEntity.h"
#include "EnemyProjectile.h"

// Entity that serves as enemy to player.
class Enemy : public DynamicEntity {
public:
  Enemy(SDL_Renderer *renderer, int startingX, int startingY, int difficulty);
  ~Enemy();

  // Grab image and create texture
  bool Initialize() override;
  // Render entity
  void Render() override;
  // Perform predefined movement
  void Move();
  // Return projectile (so it doesn't disappear when enemy dies)
  std::unique_ptr<EnemyProjectile> Fire();

private:
  // Base x-velocity
  double m_baseXVel = 0.1;

  // How long between shots (if lucky)
  int m_shotTimeout = 1000; // ms
  int m_randNumMod = 200;

  // Which direction to go
  bool m_leftCrawl = true;
  
  using Clock = std::chrono::high_resolution_clock;
  using TimePoint = std::chrono::time_point<Clock>;

  // Stores last time a shot was fired
  TimePoint m_lastFire;
};

#endif // ENEMY_H
