#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "DynamicEntity.h"

class EnemyProjectile : public DynamicEntity {
public:
  EnemyProjectile(SDL_Renderer *renderer, bool shouldBeRendered, int startingX,
             int startingY);
  ~EnemyProjectile(){};

  bool Initialize() override;
  void Render() override;

private:
  bool m_shouldBeRendered = false;
};

#endif // PROJECTILE_H