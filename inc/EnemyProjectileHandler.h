#ifndef ENEMY_PROJECTILE_HANDLER_H
#define ENEMY_PROJECTILE_HANDLER_H

#include <iostream>
#include <deque>

#include "AbstractEntity.h"
#include "EnemyProjectile.h"

// Side class that handles storing and rendering enemy projectiles.
class EnemyProjectileHandler : public AbstractEntity {
public:
  EnemyProjectileHandler(SDL_Renderer *renderer);
  ~EnemyProjectileHandler() {}

  bool Initialize() override {
      return true;
  };

  void Render() override;

  void AddNewProjectile(std::unique_ptr<EnemyProjectile> newProjectile);

  inline std::unique_ptr<EnemyProjectile>& GetProjectileReference(int idx) {
      return m_enemyProjectiles[idx];
  }

  inline size_t GetNumProjectiles() { return m_enemyProjectiles.size(); }

private:
  using EnemyProjectiles = std::deque<std::unique_ptr<EnemyProjectile>>;
  EnemyProjectiles m_enemyProjectiles;
};

#endif // ENEMY_PROJECTILE_HANDLER_H