#include "Player.h"

namespace {
constexpr int k_crop = 100;
constexpr int k_width = 200;
constexpr int k_height = 200;
constexpr int k_pixelsToMove = 10;
const std::string k_filename = "../memeinvaders/eric.png";
} // namespace

Player::Player(SDL_Renderer *renderer) : m_renderer(renderer) {
  // Player should spawn in bottom-middle of screen
  m_xPos = SCREEN_WIDTH / 2 - k_width;
  m_yPos = SCREEN_HEIGHT - k_height;

  m_screenBox = new SDL_Rect{k_crop, k_crop, SCREEN_WIDTH, SCREEN_HEIGHT};
  m_textureBox = new SDL_Rect{m_xPos, m_yPos, k_width, k_height};
}

Player::~Player() {
  // Clean up boxes
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

void Player::Render() {
  SDL_RenderCopy(m_renderer, m_texture, &*m_screenBox, &*m_textureBox);
  // Surely there must be an easier way to do this
  m_textureBox->x = m_xPos;
  m_textureBox->y = m_yPos;
}

void Player::Move(SDL_Event *e) {
  // Handle all directions and bounds check
  // Need to upgrade this to use SDL_GetKeyState for better handling multiple
  // inputs
  if (e->key.keysym.sym == SDLK_LEFT && m_xPos > k_pixelsToMove) {
    m_xPos -= k_pixelsToMove;
  }
  if (e->key.keysym.sym == SDLK_RIGHT &&
      m_xPos < SCREEN_WIDTH - k_pixelsToMove) {
    m_xPos += k_pixelsToMove;
  }
  if (e->key.keysym.sym == SDLK_UP && m_yPos > k_pixelsToMove) {
    m_yPos -= k_pixelsToMove;
  }
  if (e->key.keysym.sym == SDLK_DOWN &&
      m_yPos < SCREEN_HEIGHT - k_pixelsToMove) {
    m_yPos += k_pixelsToMove;
  }
}


void Player::Fire() {}
