#ifndef ENEMY_H
#define ENEMY_H

#include <chrono>
#include <deque>
#include <iostream>
#include <memory>
#include <vector>

#include "DynamicEntity.h"
#include "EnemyProjectile.h"

// Entity that serves as enemy to player.
class Enemy : public DynamicEntity {
public:
  Enemy(SDL_Renderer *renderer);
  ~Enemy();

  // Grab image and create texture
  bool Initialize() override;
  // Render entity
  void Render() override;
  // Perform predefined movement
  void Move();
  // Fire projectile
  void Fire();

private:
  // Stores all projectiles enemy can fire
  // Use unique_ptr because raw pointers scare me
  std::deque<std::unique_ptr<EnemyProjectile>> m_projectileArray;

  // Keeps track of shots on screen
  int m_shotsPresent = 0;

  //movement tracker
  bool hit_left = false;
  bool hit_right = true;
  int y_counter = 0;
  
  using Clock = std::chrono::high_resolution_clock;
  using TimePoint = std::chrono::time_point<Clock>;

  // Stores last time a shot was fired
  TimePoint m_lastFire;
};

#endif // ENEMY_H
