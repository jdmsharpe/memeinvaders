#include "Enemy.h"

namespace {
constexpr int k_width = 100;
constexpr int k_height = 150;
constexpr double k_baseVel = 0.1;
constexpr double k_baseVely = 5;
const std::string k_filename = "../memeinvaders/assets/enemy1.png";
constexpr int k_projectileHeightLimit = k_height;
constexpr int k_maxProjectiles = 5;
constexpr int k_shotTimeout = 250; // ms
} // namespace

Enemy::Enemy(SDL_Renderer *renderer) : DynamicEntity(renderer, "enemy") {
  // Enemy should spawn in bottom-middle of screen
  SetPosition(SCREEN_WIDTH / 2, 0);
  SetVelocity(0.0, 0.0);

  m_screenBox = new SDL_Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  m_textureBox = new SDL_Rect{m_xPos, m_yPos, k_width, k_height};
}

Enemy::~Enemy() {}

bool Enemy::Initialize() {
  // Something really went wrong
  if (m_renderer == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                 "Enemy pointer to renderer was NULL!");
    return false;
  }

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Image could not initialize!");
    return false;
  }

  // Load image and create texture
  SDL_Surface *surface = IMG_Load(k_filename.c_str());
  if (surface == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                 "Unable to load image %s! Error code: %s.", k_filename.c_str(),
                 SDL_GetError());
    return false;
  }
  m_texture = SDL_CreateTextureFromSurface(m_renderer, surface);
  SDL_FreeSurface(surface);
  return true;
}

void Enemy::Render() {
  SDL_RenderCopy(m_renderer, m_texture, &*m_screenBox, &*m_textureBox);
  // Surely there must be an easier way to do this
  m_textureBox->x = m_xPos;
  m_textureBox->y = m_yPos;

  // Take care of rendering and destroying shots
  for (int i = 0; i < m_shotsPresent; ++i) {
    // If projectile is at bottom of screen, delete it and decrement shots present
    if (m_projectileArray[i]->GetPosition().second > SCREEN_HEIGHT) {
      m_projectileArray.erase(m_projectileArray.begin() + i);
      m_shotsPresent--;
    } else {
      m_projectileArray[i]->Render();
    }
  }
}

void Enemy::Move() {
  // Handle all directions and bounds check
  // (rule is don't let sprite go off screen)
  double xVelSum = 0.0;
  double yVelSum = 0.0;
  // bool hit_left = false;
  // bool hit_right = false;
  // THIS NEEDS OPTIMIZATION
  if (m_xPos > 0) {
    if (hit_right == true) {
      // Hit left wall
      xVelSum -= k_baseVel;
      if (m_xPos == 1) {
        yVelSum += k_baseVely;
        hit_left = true;
        hit_right = false;
      }
    }
  }

  if (m_xPos < SCREEN_WIDTH - k_width) {
    // hit left wall
    if (hit_left == true) {
      xVelSum += k_baseVel;
      if (m_xPos == SCREEN_WIDTH - k_width - 1) {
        yVelSum += k_baseVely;
        hit_left = false;
        hit_right = true;
      }
    }
  }

  // if (keyboardState[SDL_SCANCODE_UP] && m_yPos > 0) {
  //   yVelSum -= k_baseVel;
  // }
  // if (keyboardState[SDL_SCANCODE_DOWN] &&
  //     m_yPos < SCREEN_HEIGHT - k_height) {
  //   yVelSum += k_baseVel;
  // }
  SetVelocity(xVelSum, yVelSum);
  UpdatePositionFromVelocity();
}

void Enemy::Fire() {
  // Make sure we're not going over cap
  if (m_shotsPresent <= k_maxProjectiles) {
    // Check last time we fired to slow things down a bit
    auto timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        Clock::now() - m_lastFire);
    if (timeElapsed.count() >= k_shotTimeout) {
      m_lastFire = Clock::now();
      // I should do the math on this at some point so that the projectile
      // looks like it's coming out of the ship or even the missile bays
      std::unique_ptr<EnemyProjectile> newProj =
          std::make_unique<EnemyProjectile>(
              m_renderer, true, m_xPos + k_width / 2, m_yPos + k_height / 10);
      newProj->Initialize();

      // Add projectile to array for storage
      m_projectileArray.push_back(std::move(newProj));
      // Store total number on screen
      m_shotsPresent = static_cast<int>(m_projectileArray.size());
    }
  }
}
