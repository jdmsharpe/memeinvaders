#include "Window.h"

Window::Window() {}

bool Window::Open() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError()
              << std::endl;
    return false;
  } else {
    // Create window
    m_window = SDL_CreateWindow("MemeInvaders", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                SCREEN_HEIGHT, 0);
    if (m_window == NULL) {
      std::cout << "Window could not be created. Error code: " << SDL_GetError()
                << std::endl;
      return false;
    } else {
      m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_SOFTWARE);
      // Get window surface
      m_screenSurface = SDL_GetWindowSurface(m_window);
    }
  }

  m_player = new Player(m_renderer);
  if (!m_player->Initialize()) {
    std::cout << "Failed to initialize player!" << std::endl;
    return false;
  }
  m_enemy = new Enemy(m_renderer);
  if (!m_enemy->Initialize()) {
    std::cout << "Failed to initialize enemy!" << std::endl;
    return false;
  }
  return true;
}

void Window::Close() {
  // Delete player
  delete m_player;
  m_player = nullptr;

  // Delete enemy
  delete m_enemy;
  m_enemy = nullptr;

  // Destroy renderer
  SDL_DestroyRenderer(m_renderer);
  m_renderer = NULL;

  // Deallocate surface
  SDL_FreeSurface(m_screenSurface);
  m_screenSurface = NULL;

  // Destroy window
  SDL_DestroyWindow(m_window);
  m_window = NULL;

  // Quit SDL subsystems
  SDL_Quit();
}

void Window::Render(const GameState &gameState) {
  unsigned int frameStart = SDL_GetTicks();

  SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(m_renderer);

  switch (gameState) {
  case GameState::MAIN_MENU:
    break;
  case GameState::GAME_MODE_1:
    m_player->Render();
    m_enemy->Render();
    break;
  case GameState::SETTINGS:
    break;
  }

  SDL_RenderPresent(m_renderer);

  unsigned int frameTime = SDL_GetTicks() - frameStart;

  if (DELTA_TIME > frameTime) {
    // std::cout << frameTime << std::endl;
    SDL_Delay(DELTA_TIME - frameTime);
  }
}
