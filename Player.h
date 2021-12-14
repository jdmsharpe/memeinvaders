#ifndef PLAYER_H
#define PLAYER_H

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
};

#endif // PLAYER_H
