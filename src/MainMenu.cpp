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
  // Something really went wrong
  if (m_renderer == NULL) {
    std::cout << "MainMenu pointer to renderer was NULL!" << std::endl;
    return false;
  }

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    std::cout << "SDL_Image could not initialize!" << std::endl;
    return false;
  }

  // Load image and create texture
  SDL_Surface *surface = IMG_Load(k_filename.c_str());
  if (surface == NULL) {
    std::cout << "Unable to load image " << k_filename
              << "! SDL Error: " << SDL_GetError() << std::endl;
    return false;
  }

  m_texture = SDL_CreateTextureFromSurface(m_renderer, surface);
  SDL_FreeSurface(surface);
  return true;
}

void MainMenu::Render() {
  SDL_RenderCopy(m_renderer, m_texture, &*m_screenBox, &*m_textureBox);
}
