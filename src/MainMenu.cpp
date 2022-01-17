#include "MainMenu.h"

namespace {
const std::string k_filename = "../memeinvaders/assets/logo.png";

constexpr int k_width = 200;
constexpr int k_height = 200;
constexpr int k_widthOffset = 100;
constexpr int k_heightOffset = 25;
} // namespace

MainMenu::MainMenu(SDL_Renderer *renderer)
    : AbstractEntity(renderer, "main menu") {

  // Logo fills top-half of screen
  m_screenBox = new SDL_Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  m_textureBox = new SDL_Rect{k_widthOffset, k_heightOffset,
                              SCREEN_WIDTH - k_width, SCREEN_HEIGHT - k_height};
}

MainMenu::~MainMenu() {}

bool MainMenu::Initialize() {
  return CreateTexture(k_filename);
}

void MainMenu::Render() {
  SDL_RenderCopy(m_renderer, m_texture, &*m_screenBox, &*m_textureBox);
}
