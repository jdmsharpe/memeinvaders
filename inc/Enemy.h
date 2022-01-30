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
  // Fire projectile
  void Fire();

  // Stores all projectiles enemy can fire
  // Use unique_ptr because raw pointers scare me
  std::deque<std::unique_ptr<EnemyProjectile>> m_projectileArray;

private:
  // Keeps track of shots on screen
  int m_shotsPresent = 0;

  // Base x-velocity
  double m_baseXVel = 0.1;

  // How long between shots (if lucky)
  int m_shotTimeout = 1000; // ms
  int m_randNumMod = 200;

  bool m_opposite = true;
  //movement tracker
  // bool hit_left = false;
  // bool hit_right = true;
  // int y_counter = 0;
  
  using Clock = std::chrono::high_resolution_clock;
  using TimePoint = std::chrono::time_point<Clock>;

  // Stores last time a shot was fired
  TimePoint m_lastFire;
};

#endif // ENEMY_H
