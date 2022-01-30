#include "Projectile.h"

namespace {
constexpr int k_width = 15;
constexpr int k_height = 45;
// const std::string k_filename = "../memeinvaders/assets/enemy1.png";
} // namespace

Projectile::Projectile(SDL_Renderer *renderer, bool shouldBeRendered,
                       int startingX, int startingY)
    : DynamicEntity(renderer, "projectile", k_width, k_height),
      m_shouldBeRendered(shouldBeRendered) {
  SetPosition(startingX, startingY);
  SetVelocity(0.0, -0.75);

  // m_screenBox = new SDL_Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  // m_textureBox = new SDL_Rect{m_xPos, m_yPos, k_width, k_height};
}

bool Projectile::Initialize() {
  // SDL_Surface *surface = IMG_Load(k_filename.c_str());
  // m_texture = SDL_CreateTextureFromSurface(m_renderer, surface);
  // SDL_FreeSurface(surface);
  // if (SDL_GetError != 0) {
  //   return false;
  // }
  // return true;

  // For now do nothing
  return true;
}

void Projectile::Render() {
  UpdatePositionFromVelocity();
  if (m_shouldBeRendered) {
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(m_renderer, m_xPos, m_yPos, m_xPos, m_yPos + k_height);
    // // Surely there must be an easier way to do this
    // m_textureBox->x = m_xPos;
    // m_textureBox->y = m_yPos;
  }
}