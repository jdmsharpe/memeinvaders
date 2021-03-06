#include "Player.h"

namespace {
constexpr int k_width = 75;
constexpr int k_height = 113;
constexpr double k_baseVel = 0.35;
const std::string k_filename = "../memeinvaders/assets/player1.png";
constexpr int k_projectileHeightLimit = -k_height;
constexpr int k_maxProjectiles = 2;
constexpr int k_shotTimeout = 250; // ms
constexpr int k_livesXOffset = 15;
constexpr int k_livesSpacing = 40;
} // namespace

Player::Player(SDL_Renderer *renderer)
    : DynamicEntity(renderer, "player", k_width, k_height) {
  // Player should spawn in bottom-middle of screen
  SetPosition(SCREEN_WIDTH / 2 - k_width, SCREEN_HEIGHT - k_height);
  SetVelocity(0.0, 0.0);

  m_screenBox = new SDL_Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  m_textureBox = new SDL_Rect{m_xPos, m_yPos, k_width, k_height};

  // Create life display entities
  for (int i = 0; i < m_lives; ++i) {
    std::unique_ptr<Lives> newLife = std::make_unique<Lives>(
        m_renderer, k_livesXOffset + k_livesSpacing * i, 0);
    newLife->Initialize();
    m_livesArray.push_back(std::move(newLife));
  }
}

Player::~Player() {}

bool Player::Initialize() {
  return CreateTexture(k_filename);
}

void Player::Render() {
  SDL_RenderCopy(m_renderer, m_texture, &*m_screenBox, &*m_textureBox);
  // Surely there must be an easier way to do this
  m_textureBox->x = m_xPos;
  m_textureBox->y = m_yPos;

  // Take care of rendering and destroying shots
  for (int i = 0; i < m_shotsPresent; ++i) {
    // If projectile is null, it's collided with something
    if (!m_projectileArray[i]) {
      m_projectileArray.erase(m_projectileArray.begin() + i);
      m_shotsPresent--;
      continue;
    }
    // If projectile is at top of screen, delete it and decrement shots present
    if (m_projectileArray[i]->GetPosition().second < k_projectileHeightLimit) {
      m_projectileArray.erase(m_projectileArray.begin() + i);
      m_shotsPresent--;
      continue;
    }

    m_projectileArray[i]->Render();
  }

  for (size_t i = 0; i < m_livesArray.size(); ++i) {
    CONTINUE_IF_NULL(m_livesArray[i]);
    m_livesArray[i]->Render();
  }
}

void Player::Move(const Uint8 *keyboardState) {
  // Handle all directions and bounds check
  // (rule is don't let sprite go off screen)
  double xVelSum = 0.0;
  double yVelSum = 0.0;
  if (keyboardState[SDL_SCANCODE_LEFT] && m_xPos > 0) {
    xVelSum -= k_baseVel;
  }
  if (keyboardState[SDL_SCANCODE_RIGHT] &&
      m_xPos < SCREEN_WIDTH - k_width) {
    xVelSum += k_baseVel;
  }
  if (keyboardState[SDL_SCANCODE_UP] && m_yPos > 0) {
    yVelSum -= k_baseVel;
  }
  if (keyboardState[SDL_SCANCODE_DOWN] &&
      m_yPos < SCREEN_HEIGHT - k_height) {
    yVelSum += k_baseVel;
  }
  SetVelocity(xVelSum, yVelSum);
  UpdatePositionFromVelocity();
}

void Player::Fire() {
  // Make sure we're not going over cap
  if (m_shotsPresent < k_maxProjectiles) {
    // Check last time we fired to slow things down a bit
    auto timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        Clock::now() - m_lastFire);
    if (timeElapsed.count() >= k_shotTimeout) {
      m_lastFire = Clock::now();
      // I should do the math on this at some point so that the projectile
      // looks like it's coming out of the ship or even the missile bays
      std::unique_ptr<Projectile> newProj = std::make_unique<Projectile>(
          m_renderer, true, m_xPos + k_width / 2, m_yPos - k_height / 4);
      newProj->Initialize();
      // Add projectile to array for storage
      m_projectileArray.push_back(std::move(newProj));
      // Store total number on screen
      m_shotsPresent = static_cast<int>(m_projectileArray.size());
    }
  }
}

void Player::UpdateLivesDisplay() {
  // If player lost a life, remove last member of vector
  if (GetLives() < (int)m_livesArray.size()) {
    m_livesArray.erase(m_livesArray.end() - 1);
  // If player gained a life, create a new member
  } else if (GetLives() > (int)m_livesArray.size()) {
    std::unique_ptr<Lives> newLife = std::make_unique<Lives>(
        m_renderer, k_livesXOffset + (GetLives() - 1) * k_livesSpacing, 0);
    newLife->Initialize();
    m_livesArray.push_back(std::move(newLife));
  }
}

void Player::ResetPosition() {
  SetPosition(SCREEN_WIDTH / 2 - k_width / 2, SCREEN_HEIGHT - k_height);
}
