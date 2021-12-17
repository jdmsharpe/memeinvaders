#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Entity.h"

class Projectile : public Entity {
public:
  Projectile(SDL_Renderer *renderer, bool shouldBeRendered, int startingX,
             int startingY);
  ~Projectile(){};

  bool Initialize() override;
  void Render() override;

private:
  bool m_shouldBeRendered = false;
};

#endif // PROJECTILE_H