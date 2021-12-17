#ifndef ENEMY_H
#define ENEMY_H

#include <chrono>
#include <deque>
#include <iostream>
#include <memory>
#include <vector>

#include "DynamicEntity.h"
#include "Projectile.h"

// Entity that serves as enemy to player.
class Enemy : public DynamicEntity {
public:
  Enemy(SDL_Renderer *renderer);
  ~Enemy();

  // Grab image and create texture
  bool Initialize() override;
  // Render entity
  void Render() override;
  // Process keyboard input
  void Move();
  // Fire projectile
  void Fire();

private:
  // Stores all projectiles enemy can fire
  // Use unique_ptr because raw pointers scare me
  //std::deque<std::unique_ptr<Projectile>> m_projectileArray;

  // Keeps track of shots on screen
  //int m_shotsPresent = 0;

  using Clock = std::chrono::high_resolution_clock;
  using TimePoint = std::chrono::time_point<Clock>;

  // Stores last time a shot was fired
  TimePoint m_lastFire;
};

#endif // PLAYER_H
