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

std::unique_ptr<Enemy> CreateAndInitializeEnemy(SDL_Renderer *renderer,
                                                int startingX, int startingY) {
  std::unique_ptr<Enemy> toReturn =
      std::make_unique<Enemy>(renderer, startingX, startingY);
  if (!toReturn->Initialize()) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to initialize %s!",
                 toReturn->GetName().c_str());
    return nullptr;
  }
  return std::move(toReturn);
}

const std::vector<std::pair<int, int>> k_enemyMap = {
    {SCREEN_WIDTH / 2 + 600, 0},
    {SCREEN_WIDTH / 2 + 400, 0},
    {SCREEN_WIDTH / 2 + 200, 0},
    {SCREEN_WIDTH / 2 + 0, 0},
    {SCREEN_WIDTH / 2 - 200, 0},
    {SCREEN_WIDTH / 2 - 400, 0},
    {SCREEN_WIDTH / 2 - 600, 0},
    {SCREEN_WIDTH / 2 + 600, 200},
    {SCREEN_WIDTH / 2 + 400, 200},
    {SCREEN_WIDTH / 2 + 200, 200},
    {SCREEN_WIDTH / 2, 200},
    {SCREEN_WIDTH / 2 - 200, 200},
    {SCREEN_WIDTH / 2 - 400, 200},
    {SCREEN_WIDTH / 2 - 600, 200},
    };
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
      return false;
    } else {
      m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_SOFTWARE);
      m_screenSurface = SDL_GetWindowSurface(m_window);
    }
  }

  // Something really went wrong
  if (m_renderer == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                "MainMenu pointer to renderer was NULL!");
    return false;
  }

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Image could not initialize!");
    return false;
  }

  if (TTF_Init() == -1) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_TTF could not initialize!");
    return false;
  }

  CreateEntities();

  return true;
}

void Window::CreateEntities() {
  m_mainMenu = CreateAndInitialize<MainMenu>(m_renderer);
  m_player = CreateAndInitialize<Player>(m_renderer);

  for (size_t i = 0; i < m_startingNumEnemies; ++i) {
    m_enemies.push_back(CreateAndInitializeEnemy(
        m_renderer, k_enemyMap[i].first, k_enemyMap[i].second));
  }

  m_score = CreateAndInitialize<Score>(m_renderer);
}

void Window::ResetGameMode1() {
  // Reset player and lives
  m_player.reset();
  m_player = CreateAndInitialize<Player>(m_renderer);

  // Clear and repopulate vector of enemies
  m_numEnemies = m_startingNumEnemies;
  m_enemies.clear();

  for (size_t i = 0; i < m_numEnemies; ++i) {
    m_enemies.push_back(CreateAndInitializeEnemy(
        m_renderer, k_enemyMap[i].first, k_enemyMap[i].second));
  }

  m_score->ResetScore();
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
    for (int i = 0; i < m_startingNumEnemies; ++i) {
      EXECUTE_IF_VALID(m_enemies[i], m_enemies[i]->Render());
      EXECUTE_IF_BOTH_VALID(m_player, m_enemies[i], CollisionDetection(i));
    }
    EXECUTE_IF_VALID(m_score, m_score->Render());
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

void Window::CollisionDetection(int enemyIdx) {
  // Initialize flags and get bounding box shorthands
  bool killPlayer = false;
  bool killEnemy = false;
  Enemy* enemy = m_enemies[enemyIdx].get();
  BoundingBox *playerBox = m_player->GetBoundingBox();
  BoundingBox *enemyBox = enemy->GetBoundingBox();

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
  for (size_t j = 0; j < enemy->m_projectileArray.size(); j++) {
    CONTINUE_IF_NULL(enemy->m_projectileArray[j]);
    BoundingBox *enemyProjectileBox =
        enemy->m_projectileArray[j]->GetBoundingBox();
    // Separating axis test
    if (playerBox->bottom > enemyProjectileBox->top &&
        playerBox->top < enemyProjectileBox->bottom &&
        playerBox->right > enemyProjectileBox->left &&
        playerBox->left < enemyProjectileBox->right) {
      killPlayer = true;
      // Delete projectile
      enemy->m_projectileArray[j].reset();
    }
  }

  if (killPlayer) {
    // Decrement one life
    m_player->SetLives(m_player->GetLives() - 1);
    // Dead after all lives are gone
    if (m_player->GetLives() <= 0) {
      m_player.reset();
    }
    m_score->UpdateScore(-5000);
  }

  if (killEnemy) {
    m_enemies[enemyIdx].reset();
    m_numEnemies--;
    m_score->UpdateScore(1000);
  }

  if (m_score->Give1Up()) {
    m_player->SetLives(m_player->GetLives() + 1);
  }
  m_player->UpdateLivesDisplay();
}