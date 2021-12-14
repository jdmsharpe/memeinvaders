#include "Projectile.h"

namespace {
constexpr int k_length = 20;
constexpr int k_amountToMoveUp = 10;
} // namespace

Projectile::Projectile(SDL_Renderer *renderer, bool shouldBeRendered,
                       int startingX, int startingY)
    : Entity(renderer), m_shouldBeRendered(shouldBeRendered) {
  SetPosition(std::make_pair(startingX, startingY));
}

void Projectile::Render() {
  if (m_shouldBeRendered) {
      SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
      SDL_RenderDrawLine(m_renderer, m_xPos, m_yPos, m_xPos, m_yPos - k_length);
  }
  m_yPos -= k_amountToMoveUp;
}