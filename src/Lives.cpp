#include "Lives.h"

namespace {
const std::string k_filename = "../memeinvaders/assets/player1.png";
constexpr int k_width = 30;
constexpr int k_height = 45;
} // namespace

Lives::Lives(SDL_Renderer *renderer, int startingX, int startingY)
    : AbstractEntity(renderer, "lives") {

  m_screenBox = new SDL_Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  m_textureBox = new SDL_Rect{startingX, startingY, k_width, k_height};
}

Lives::~Lives() {}

bool Lives::Initialize() {
  return CreateTexture(k_filename);
}

void Lives::Render() {
  SDL_RenderCopy(m_renderer, m_texture, &*m_screenBox, &*m_textureBox);
}
