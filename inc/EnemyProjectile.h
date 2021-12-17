#ifndef ENEMYPROJECTILE_H
#define ENEMYPROJECTILE_H
#include <iostream>
#include "DynamicEntity.h"

class EnemyProjectile : public DynamicEntity {
public:
  EnemyProjectile(SDL_Renderer *renderer, bool shouldBeRendered, int startingX,
             int startingY);
  ~EnemyProjectile(){};

  bool Initialize() override;
  void Render() override;

private:
  bool m_shouldBeRendered = true;
};

#endif // PROJECTILE_H
