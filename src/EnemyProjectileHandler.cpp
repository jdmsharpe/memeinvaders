#include "EnemyProjectileHandler.h"

namespace {
constexpr int k_projectileHeightLimit = SCREEN_HEIGHT;
} // namespace

EnemyProjectileHandler::EnemyProjectileHandler(SDL_Renderer *renderer)
    : AbstractEntity(renderer, "projectile handler") {}

void EnemyProjectileHandler::Render() {
  // Take care of rendering and destroying shots
  for (size_t i = 0; i < m_enemyProjectiles.size(); ++i) {
    // If projectile is null, it's collided with something
    if (!m_enemyProjectiles[i]) {
      m_enemyProjectiles.erase(m_enemyProjectiles.begin() + i);
      continue;
    }
    // If projectile is at top of screen, delete it and decrement shots present
    if (m_enemyProjectiles[i]->GetPosition().second > k_projectileHeightLimit) {
      m_enemyProjectiles.erase(m_enemyProjectiles.begin() + i);
      continue;
    }

    m_enemyProjectiles[i]->Render();
  }
}

void EnemyProjectileHandler::AddNewProjectile(
    std::unique_ptr<EnemyProjectile> newProjectile) {
  m_enemyProjectiles.push_back(std::move(newProjectile));
}


