#ifndef PLAYER_H
#define PLAYER_H

#include <chrono>
#include <iostream>
#include <vector>

#include "DynamicEntity.h"
#include "Projectile.h"

// Entity controlled by player during gameplay.
class Player : public DynamicEntity {
public:
  Player(SDL_Renderer *renderer);
  ~Player();

  // Grab image and create texture
  bool Initialize() override;
  // Render entity
  void Render() override;
  // Process keyboard input
  void Move(const Uint8* keyboardState);
  // Fire projectile
  void Fire();

  inline int GetLives() {
    return m_lives;
  }

  inline void GiveLives(int amount) {
    m_lives += amount;
  }

  // Stores all projectiles player can fire
  // Use unique_ptr because raw pointers scare me
  std::vector<std::unique_ptr<Projectile>> m_projectileArray;

private:
  // 3 lives to start
  int m_lives = 3;

  // Keeps track of shots on screen
  int m_shotsPresent = 0;

  using Clock = std::chrono::high_resolution_clock;
  using TimePoint = std::chrono::time_point<Clock>;

  // Stores last time a shot was fired
  TimePoint m_lastFire;
};

#endif // PLAYER_H
