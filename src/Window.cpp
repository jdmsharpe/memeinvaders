#include "Window.h"

namespace {
template <class T>
std::unique_ptr<T> CreateAndInitialize(SDL_Renderer *renderer) {
  std::unique_ptr<T> toReturn = std::make_unique<T>(renderer);
  if (!toReturn->Initialize()) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to initialize %s!",
                 toReturn->GetName().c_str());
    return nullptr;
  }
  return std::move(toReturn);
}
} // namespace

Window::Window() {}

bool Window::Open() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                 "SDL could not initialize! Error code: %s.", SDL_GetError());
    return false;
  } else {
    // Create window
    m_window = SDL_CreateWindow("MemeInvaders", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                SCREEN_HEIGHT, 0);
    if (m_window == NULL) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                   "Window could not be created. Error code: %s.",
                   SDL_GetError());
    } else {
      m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_SOFTWARE);
      m_screenSurface = SDL_GetWindowSurface(m_window);
    }
  }

  m_mainMenu = CreateAndInitialize<MainMenu>(m_renderer);
  m_player = CreateAndInitialize<Player>(m_renderer);
  m_enemy = CreateAndInitialize<Enemy>(m_renderer);

  return true;
}

void Window::Close() {
  // Clean up all entities and SDL objects
  SDL_DestroyRenderer(m_renderer);
  m_renderer = NULL;

  SDL_FreeSurface(m_screenSurface);
  m_screenSurface = NULL;

  SDL_DestroyWindow(m_window);
  m_window = NULL;

  SDL_Quit();
}

void Window::Render(const GameState &gameState) {
  unsigned int frameStart = SDL_GetTicks();

  // Black background
  SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(m_renderer);

  switch (gameState) {
  case GameState::MAIN_MENU:
    EXECUTE_IF_VALID(m_mainMenu, m_mainMenu->Render());
    break;
  case GameState::GAME_MODE_1:
    EXECUTE_IF_VALID(m_player, m_player->Render());
    EXECUTE_IF_VALID(m_enemy, m_enemy->Render());
    EXECUTE_IF_BOTH_VALID(m_player, m_enemy, CollisionDetection());
    break;
  case GameState::SETTINGS:
    break;
  }

  SDL_RenderPresent(m_renderer);

  unsigned int frameTime = SDL_GetTicks() - frameStart;

  // If actual time taken is less than defined frame time,
  // wait a bit until they're equal (fixed framerate)
  if (DELTA_TIME > frameTime) {
    // Debug print to check actual elapsed time
    // SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Elapsed frame time was %d
    // ms.", frameTime);
    SDL_Delay(DELTA_TIME - frameTime);
  }
}

void Window::CollisionDetection() {
  // Initialize flags and get bounding box shorthands
  bool killPlayer = false;
  bool killEnemy = false;
  BoundingBox *playerBox = m_player->GetBoundingBox();
  BoundingBox *enemyBox = m_enemy->GetBoundingBox();

  // Loop through all player-filed projectiles and check for enemy collision
  // TODO: need extra for-loop to account for multiple enemies
  for (size_t i = 0; i < m_player->m_projectileArray.size(); i++) {
    CONTINUE_IF_NULL(m_player->m_projectileArray[i]);
    BoundingBox *projectileBox =
        m_player->m_projectileArray[i]->GetBoundingBox();
    // Separating axis test
    if (enemyBox->bottom > projectileBox->top &&
        enemyBox->top < projectileBox->bottom &&
        enemyBox->right > projectileBox->left &&
        enemyBox->left < projectileBox->right) {
      killEnemy = true;
      // Delete projectile
      m_player->m_projectileArray[i].reset();
    }
  }

  // Loop through all enemy-fired projectiles and check for enemy collision
  // TODO: need extra for-loop to account for multiple enemies
  for (size_t j = 0; j < m_enemy->m_projectileArray.size(); j++) {
    CONTINUE_IF_NULL(m_enemy->m_projectileArray[j]);
    BoundingBox *enemyProjectileBox =
        m_enemy->m_projectileArray[j]->GetBoundingBox();
    // Separating axis test
    if (playerBox->bottom > enemyProjectileBox->top &&
        playerBox->top < enemyProjectileBox->bottom &&
        playerBox->right > enemyProjectileBox->left &&
        playerBox->left < enemyProjectileBox->right) {
      killPlayer = true;
      // Delete projectile
      m_enemy->m_projectileArray[j].reset();
    }
  }

  if (killPlayer) {
    // Decrement one life
    m_player->GiveLives(-1);
    // Dead after all lives are gone
    if (m_player->GetLives() <= 0) {
      m_player.reset();
    }
  }

  if (killEnemy) {
    m_enemy.reset();
  }
}