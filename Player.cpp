#include "Player.h"

namespace {
constexpr int k_width = 200;
constexpr int k_height = 200;
constexpr int k_pixelsToMove = 5;
const std::string k_filename = "../memeinvaders/eric.bmp";
} // namespace

Player::Player(SDL_Renderer *renderer) : m_renderer(renderer) {
  // Player should spawn in bottom-middle of screen
  m_xPos = SCREEN_WIDTH / 2;
  m_yPos = SCREEN_HEIGHT - (k_height / 2);

  m_screenBox = new SDL_Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  m_textureBox = new SDL_Rect{m_xPos, m_yPos, k_width, k_height};
}

Player::~Player() {
  delete m_screenBox;
  m_screenBox = NULL;

  delete m_textureBox;
  m_textureBox = NULL;

  // Clean up texture
  SDL_DestroyTexture(m_texture);
  m_texture = NULL;
}

bool Player::Initialize() {
  // Something really went wrong
  if (m_renderer == NULL) {
    std::cout << "Player pointer to renderer was NULL!" << std::endl;
    return false;
  }

  // Load image and create texture
  SDL_Surface *surface = SDL_LoadBMP(k_filename.c_str());
  if (surface == NULL) {
    std::cout << "Unable to load image " << k_filename
              << "! SDL Error: " << SDL_GetError() << std::endl;
    return false;
  }
  m_texture = SDL_CreateTextureFromSurface(m_renderer, surface);
  SDL_FreeSurface(surface);
  return true;
}

void Player::Render() {
  SDL_RenderCopy(m_renderer, m_texture, &*m_textureBox, &*m_screenBox);
}

void Player::Move(SDL_Event *e) {
  // Handle all directions and bounds check
  if (e->key.keysym.sym == SDLK_LEFT && m_xPos > k_pixelsToMove) {
    m_xPos -= k_pixelsToMove;
    std::cout << "Moving left" << std::endl;
  }
  if (e->key.keysym.sym == SDLK_RIGHT &&
      m_xPos < SCREEN_WIDTH - k_pixelsToMove) {
    m_xPos += k_pixelsToMove;
    std::cout << "Moving right" << std::endl;
  }
  if (e->key.keysym.sym == SDLK_UP && m_yPos > k_pixelsToMove) {
    m_yPos -= k_pixelsToMove;
    std::cout << "Moving up" << std::endl;
  }
  if (e->key.keysym.sym == SDLK_DOWN &&
      m_yPos < SCREEN_HEIGHT - k_pixelsToMove) {
    m_yPos += k_pixelsToMove;
    std::cout << "Moving down" << std::endl;
  }
}

void Player::Fire() {}
