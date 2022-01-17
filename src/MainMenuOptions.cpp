#include "MainMenuOptions.h"

namespace {
const std::string k_filename = "../memeinvaders/assets/new_game.png";
constexpr int k_width = 1100;
constexpr int k_height = 700;
constexpr int k_widthOffset = 550;
constexpr int k_heightOffset = 600;
} // namespace

MainMenuOptions::MainMenuOptions(SDL_Renderer *renderer)
    : AbstractEntity(renderer, "main menu") {

  // Logo fills top-half of screen
  //m_screenBox = new SDL_Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  m_textureBox = new SDL_Rect{k_widthOffset, k_heightOffset,
                              SCREEN_WIDTH - k_width, SCREEN_HEIGHT - k_height};
}

MainMenuOptions::~MainMenuOptions() {}

bool MainMenuOptions::Initialize() {
  return CreateTexture(k_filename);
}

void MainMenuOptions::Render() {
  SDL_RenderCopy(m_renderer, m_texture, &*m_screenBox, &*m_textureBox);
}
