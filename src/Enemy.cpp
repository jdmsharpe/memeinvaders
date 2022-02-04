#include "Enemy.h"

namespace {
constexpr int k_width = 75;
constexpr int k_height = 113;
constexpr double k_baseYVel = 4.75;
const std::string k_filename = "../memeinvaders/assets/enemy1.png";
constexpr int k_randNumMod = 200;
constexpr int k_offscreenEnough = 10;
} // namespace

Enemy::Enemy(SDL_Renderer *renderer, int startingX, int startingY, int difficulty)
    : DynamicEntity(renderer, "enemy", k_width, k_height) {
  // Enemy should spawn in bottom-middle of screen
  SetPosition(startingX, startingY);

  // Take into account the current level...
  m_baseXVel *= difficulty;
  m_shotTimeout /= difficulty;
  m_randNumMod /= difficulty;

  // ...for initial velocity
  SetVelocity(m_baseXVel, 0.0);

  m_screenBox = new SDL_Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  m_textureBox = new SDL_Rect{m_xPos, m_yPos, k_width, k_height};
}

Enemy::~Enemy() {}

bool Enemy::Initialize() { return CreateTexture(k_filename); }

void Enemy::Render() {
  SDL_RenderCopy(m_renderer, m_texture, &*m_screenBox, &*m_textureBox);
  // Surely there must be an easier way to do this
  m_textureBox->x = m_xPos;
  m_textureBox->y = m_yPos;
}

void Enemy::Move() {
  double xVelSum = m_baseXVel;
  double yVelSum = 0.0;
  if (m_xPos <= 0) {
    m_leftCrawl = false;
    yVelSum += k_baseYVel;
  }
  
  if (m_xPos >= SCREEN_WIDTH - k_width) {
    m_leftCrawl = true;
    yVelSum += k_baseYVel;
  }

  if (m_leftCrawl) {
    xVelSum *= -1;
  }

  SetVelocity(xVelSum, yVelSum);
  UpdatePositionFromVelocity();
}

std::unique_ptr<EnemyProjectile> Enemy::Fire() {
  // This is probably not the best
  std::unique_ptr<EnemyProjectile> newProj = nullptr;

  // Check last time we fired to slow things down a bit
  auto timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
      Clock::now() - m_lastFire);
  // Don't always fire
  int randNum = rand();
  if (timeElapsed.count() >= m_shotTimeout && randNum % m_randNumMod == 0) {
    m_lastFire = Clock::now();
    newProj = std::make_unique<EnemyProjectile>(
        m_renderer, true, m_xPos + k_width / 2, m_yPos + k_height / 10);
    newProj->Initialize();
  }

  return std::move(newProj);
}
