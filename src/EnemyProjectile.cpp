#include "EnemyProjectile.h"

namespace {
// Make it a bit smaller than the player's for now
constexpr int k_width = 20;
constexpr int k_height = 60;
const std::string k_filename = "../memeinvaders/assets/enemyproj.png";
} // namespace

EnemyProjectile::EnemyProjectile(SDL_Renderer *renderer, bool shouldBeRendered,
                       int startingX, int startingY)
    : DynamicEntity(renderer, "projectile", k_width, k_height),
      m_shouldBeRendered(shouldBeRendered) {
  // Offset position so it looks like it's coming out of the enemy
  SetPosition(startingX, startingY + k_height);
  SetVelocity(0.0, 0.625);

  m_screenBox = new SDL_Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  m_textureBox = new SDL_Rect{m_xPos, m_yPos, k_width, k_height};
}

bool EnemyProjectile::Initialize() {
  return CreateTexture(k_filename);
}

void EnemyProjectile::Render() {
  UpdatePositionFromVelocity();
  if (m_shouldBeRendered) {
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderCopy(m_renderer, m_texture, &*m_screenBox, &*m_textureBox);
    m_textureBox->x = m_xPos;
    m_textureBox->y = m_yPos;
  }
}
