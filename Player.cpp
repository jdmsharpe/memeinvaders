#include "Player.h"

namespace {
constexpr int k_maxProjectiles = 4;
constexpr int k_crop = 100;
constexpr int k_width = 200;
constexpr int k_height = 200;
constexpr int k_pixelsToMove = 10;
const std::string k_filename = "../memeinvaders/eric.png";
constexpr int k_projectileHeightLimit = 5;
} // namespace

Player::Player(SDL_Renderer *renderer) : Entity(renderer) {
  // Player should spawn in bottom-middle of screen
  m_xPos = SCREEN_WIDTH / 2 - k_width;
  m_yPos = SCREEN_HEIGHT - k_height;

  m_screenBox = new SDL_Rect{k_crop, k_crop, SCREEN_WIDTH, SCREEN_HEIGHT};
  m_textureBox = new SDL_Rect{m_xPos, m_yPos, k_width, k_height};

  // Preallocate vector of projectiles
  m_projectileArray.reserve(k_maxProjectiles);
}

Player::~Player() {}

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

  // Take care of rendering and destroying shots
  for (int i = 0; i < m_shotsPresent; ++i) {
    m_projectileArray[i]->Render();

    // Projectile is at top of screen, delete it and decrement shots present
    if (m_projectileArray[i]->GetPosition().second < k_projectileHeightLimit) {
      m_projectileArray.erase(m_projectileArray.begin() + i);
      m_shotsPresent--;
    }
  }
}

void Player::Move(const Uint8 *keyboardState) {
  // Handle all directions and bounds check
  if (keyboardState[SDL_SCANCODE_LEFT] && m_xPos > k_pixelsToMove) {
    m_xPos -= k_pixelsToMove;
  }
  if (keyboardState[SDL_SCANCODE_RIGHT] &&
      m_xPos < SCREEN_WIDTH - k_pixelsToMove) {
    m_xPos += k_pixelsToMove;
  }
  if (keyboardState[SDL_SCANCODE_UP] && m_yPos > k_pixelsToMove) {
    m_yPos -= k_pixelsToMove;
  }
  if (keyboardState[SDL_SCANCODE_DOWN] &&
      m_yPos < SCREEN_HEIGHT - k_pixelsToMove) {
    m_yPos += k_pixelsToMove;
  }
}

void Player::Fire() {
  // Make sure we're not going over cap
  if (m_projectileArray.size() <= static_cast<size_t>(k_maxProjectiles)) {
    std::unique_ptr<Projectile> newProj =
        std::make_unique<Projectile>(m_renderer, true, m_xPos + k_width / 2, m_yPos);
    // Add projectile to vector for storage
    std::cout << "Created projectile. Current size of vector is: "
              << m_projectileArray.size() << std::endl;
    m_projectileArray.push_back(std::move(newProj));
    // Store total number on screen
    m_shotsPresent = static_cast<int>(m_projectileArray.size());
  }
}
