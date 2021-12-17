#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "DynamicEntity.h"

class Projectile : public DynamicEntity {
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