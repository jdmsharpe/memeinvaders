#ifndef PLAYER_H
#define PLAYER_H

#include <chrono>
#include <iostream>
#include <memory>
#include <vector>

#include "Projectile.h"
#include "ScreenDefs.h"

class Player : public Entity {
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

private:
  // Stores all projectiles player can fire
  std::vector<std::unique_ptr<Projectile>> m_projectileArray;

  // Keeps track of shots on screen
  int m_shotsPresent = 0;

  using Clock = std::chrono::high_resolution_clock;
  using TimePoint = std::chrono::time_point<std::chrono::system_clock>;

  // Stores last time a shot was fired
  TimePoint m_lastFire;
};

#endif // PLAYER_H
