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
                                                int startingX, int startingY,
                                                int difficulty) {
  std::unique_ptr<Enemy> toReturn =
      std::make_unique<Enemy>(renderer, startingX, startingY, difficulty);
  if (!toReturn->Initialize()) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to initialize %s!",
                 toReturn->GetName().c_str());
    return nullptr;
  }
  return std::move(toReturn);
}

const auto CountRemainingEnemies =
    [](const std::vector<std::unique_ptr<Enemy>> &enemies) {
      int toReturn = 0;
      for (const auto &enemy : enemies) {
        if (enemy) {
          toReturn++;
        }
      }
      return toReturn;
    };

constexpr int k_XSeparation = 150;
constexpr int k_baseY = 10;
constexpr int k_YSeparation = 150;
constexpr int k_checkpointScore = 15000;
constexpr int k_enemyHitDeduction = -5000;

const std::vector<std::pair<int, int>> k_enemyMap = {
    {SCREEN_WIDTH / 2 + (k_XSeparation * 3), k_baseY},
    {SCREEN_WIDTH / 2 + (k_XSeparation * 2), k_baseY},
    {SCREEN_WIDTH / 2 + k_XSeparation, k_baseY},
    {SCREEN_WIDTH / 2, k_baseY},
    {SCREEN_WIDTH / 2 - k_XSeparation, k_baseY},
    {SCREEN_WIDTH / 2 - (k_XSeparation * 2), k_baseY},
    {SCREEN_WIDTH / 2 - (k_XSeparation * 3), k_baseY},
    {SCREEN_WIDTH / 2 + (k_XSeparation * 3), k_YSeparation + k_baseY},
    {SCREEN_WIDTH / 2 + (k_XSeparation * 2), k_YSeparation + k_baseY},
    {SCREEN_WIDTH / 2 + k_XSeparation, k_YSeparation + k_baseY},
    {SCREEN_WIDTH / 2, k_YSeparation + k_baseY},
    {SCREEN_WIDTH / 2 - k_XSeparation, k_YSeparation + k_baseY},
    {SCREEN_WIDTH / 2 - (k_XSeparation * 2), k_YSeparation + k_baseY},
    {SCREEN_WIDTH / 2 - (k_XSeparation * 3), k_YSeparation + k_baseY},
    {SCREEN_WIDTH / 2 + (k_XSeparation * 3), k_YSeparation * 2 + k_baseY},
    {SCREEN_WIDTH / 2 + (k_XSeparation * 2), k_YSeparation * 2 + k_baseY},
    {SCREEN_WIDTH / 2 + k_XSeparation, k_YSeparation * 2 + k_baseY},
    {SCREEN_WIDTH / 2, k_YSeparation * 2 + k_baseY},
    {SCREEN_WIDTH / 2 - k_XSeparation, k_YSeparation * 2 + k_baseY},
    {SCREEN_WIDTH / 2 - (k_XSeparation * 2), k_YSeparation * 2 + k_baseY},
    {SCREEN_WIDTH / 2 - (k_XSeparation * 3), k_YSeparation * 2 + k_baseY}};
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
  m_mainMenuOptions = CreateAndInitialize<MainMenuOptions>(m_renderer);
  m_highScore = CreateAndInitialize<HighScore>(m_renderer);
  m_player = CreateAndInitialize<Player>(m_renderer);
  m_projectileHandler = CreateAndInitialize<EnemyProjectileHandler>(m_renderer);

  for (size_t i = 0; i < m_startingNumEnemies; ++i) {
    m_enemies.push_back(CreateAndInitializeEnemy(
        m_renderer, k_enemyMap[i].first, k_enemyMap[i].second, m_level));
  }

  m_score = CreateAndInitialize<Score>(m_renderer);
}

void Window::ResetGameMode1(bool hardReset, int difficulty) {
  // Clear and repopulate vector of enemies
  m_numEnemies = m_startingNumEnemies;
  m_enemies.clear();

  for (size_t i = 0; i < m_numEnemies; ++i) {
    m_enemies.push_back(CreateAndInitializeEnemy(
        m_renderer, k_enemyMap[i].first, k_enemyMap[i].second, difficulty));
  }

  m_projectileHandler = CreateAndInitialize<EnemyProjectileHandler>(m_renderer);

  if (hardReset) {
    // Reset player, lives, and score
    m_player.reset();
    m_player = CreateAndInitialize<Player>(m_renderer);
    m_score->ResetScore();
  }

  m_player->ResetPosition();
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
    EXECUTE_IF_VALID(m_mainMenuOptions, m_mainMenuOptions->Render());
    break;
  case GameState::HIGH_SCORE:
    EXECUTE_IF_VALID(m_highScore, m_highScore->Render());
    break;
  case GameState::GAME_MODE_1:
    EXECUTE_IF_VALID(m_player, m_player->Render());
    EXECUTE_IF_VALID(m_projectileHandler, m_projectileHandler->Render());
    for (int i = 0; i < m_startingNumEnemies; ++i) {
      EXECUTE_IF_VALID(m_enemies[i], m_enemies[i]->Render());
      // First flag is for enemy, second is for player
      std::pair<bool, bool> collisionResult;
      EXECUTE_IF_BOTH_VALID(m_player, m_enemies[i],
                            CollisionDetection(i, collisionResult));
      EXECUTE_IF_VALID(m_score, ProcessEvents(i, collisionResult));
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

void Window::CollisionDetection(int enemyIdx,
                                std::pair<bool, bool> &collisionResult) {
  // Get shorthands
  Enemy *enemy = m_enemies[enemyIdx].get();
  BoundingBox *playerBox = m_player->GetBoundingBox();
  BoundingBox *enemyBox = enemy->GetBoundingBox();

  // If player touches enemy, kill player and enemy
  if (playerBox->bottom > enemyBox->top && playerBox->top < enemyBox->bottom &&
      playerBox->right > enemyBox->left && playerBox->left < enemyBox->right) {
    collisionResult.first = true;
    collisionResult.second = true;
  }

  // Loop through all player-filed projectiles and check for enemy collision
  for (size_t j = 0; j < m_player->m_projectileArray.size(); j++) {
    CONTINUE_IF_NULL(m_player->m_projectileArray[j]);
    BoundingBox *projectileBox =
        m_player->m_projectileArray[j]->GetBoundingBox();
    // Separating axis test
    // If player projectile hit enemy, kill enemy
    if (enemyBox->bottom > projectileBox->top &&
        enemyBox->top < projectileBox->bottom &&
        enemyBox->right > projectileBox->left &&
        enemyBox->left < projectileBox->right) {
      collisionResult.first = true;
      // Delete projectile
      m_player->m_projectileArray[j].reset();
    }
  }

  // Loop through all enemy-fired projectiles and check for player collision
  for (size_t k = 0; k < m_projectileHandler->GetNumProjectiles(); k++) {
    CONTINUE_IF_NULL(m_projectileHandler->GetProjectileReference(k));
    BoundingBox *enemyProjectileBox =
        m_projectileHandler->GetProjectileReference(k)->GetBoundingBox();
    // If enemy projectile hit player, kill player
    if (playerBox->bottom > enemyProjectileBox->top &&
        playerBox->top < enemyProjectileBox->bottom &&
        playerBox->right > enemyProjectileBox->left &&
        playerBox->left < enemyProjectileBox->right) {
      collisionResult.second = true;
      // Delete projectile
      m_projectileHandler->GetProjectileReference(k).reset();
    }
  }
}

void Window::ProcessEvents(int enemyIdx,
                           const std::pair<bool, bool> &collisionResult) {
  // Kill enemy
  if (collisionResult.first) {
    m_enemies[enemyIdx].reset();
    m_numEnemies = CountRemainingEnemies(m_enemies);
    m_score->UpdateScore(1000);
    m_validCheckpoint = true;
    // All enemies dead
    if (m_numEnemies == 0) {
      m_nextLevel = true;
    }
  }

  // Kill player
  if (collisionResult.second) {
    // Decrement one life
    m_player->SetLives(m_player->GetLives() - 1);
    m_player->UpdateLivesDisplay();
    // Dead after all lives are gone
    if (m_player->GetLives() <= 0) {
      m_player.reset();
      m_gameOver = true;
    }
    m_score->UpdateScore(k_enemyHitDeduction);
    m_validCheckpoint = false;
  }

  // Check if score is at checkpoint
  if (m_score->Give1Up(k_checkpointScore) && m_validCheckpoint) {
    m_player->SetLives(m_player->GetLives() + 1);
    m_player->UpdateLivesDisplay();
  }

  // Game over, player has died
  // Enter high score if applicable
  // Need actual game over screen with actual prompt
  if (m_gameOver) {
    std::string name;
    std::cout << "Please enter your name: ";
    std::cin >> name;
    m_highScore->AddEntry(std::make_pair(m_score->GetScore(), name));
    m_gameOver = false;
  }

  // Increment level and difficulty
  if (m_nextLevel) {
    m_level++;
    ResetGameMode1(false, m_level);
    m_nextLevel = false;
  }
}
