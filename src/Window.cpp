#include "Window.h"

namespace {
template <class T>
std::unique_ptr<T> CreateAndInitialize(SDL_Renderer *renderer) {
  std::unique_ptr<T> toReturn = std::make_unique<T>(renderer);
  if (!toReturn->Initialize()) {
    std::cout << "Failed to initialize " << toReturn->GetName() << "!"
              << std::endl;
    return nullptr;
  }
  return std::move(toReturn);
}
} // namespace test

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
    m_mainMenu->Render();
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

  // If actual time taken is less than defined frame time,
  // wait a bit until they're equal (fixed framerate)
  if (DELTA_TIME > frameTime) {
    // Debug print to check actual elapsed time
    //std::cout << frameTime << std::endl;
    SDL_Delay(DELTA_TIME - frameTime);
  }
}
