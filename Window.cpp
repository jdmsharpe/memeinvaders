#include "Window.h"
#include "ScreenDefs.h"

namespace {
	constexpr Uint32 k_frameDelay = 32;
}

Window::Window() {}

bool Window::Open() {
  bool success = true;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError()
              << std::endl;
    success = false;
  } else {
    // Create window
    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &m_window, &m_renderer);
    if (!m_window || !m_renderer) {
      std::cout << "Window or renderer could not be created. Error code: "
                << SDL_GetError() << std::endl;
      success = false;
    } else {
      // Get window surface
      m_screenSurface = SDL_GetWindowSurface(m_window);
    }
  }
  return success;
}

void Window::Close() {
  // Destroy renderer
  SDL_DestroyRenderer(m_renderer);
  m_renderer = nullptr;

  // Deallocate surface
  SDL_FreeSurface(m_screenSurface);
  m_screenSurface = NULL;

  // Destroy window
  SDL_DestroyWindow(m_window);
  m_window = NULL;

  // Quit SDL subsystems
  SDL_Quit();
}

void Window::Render() {
  unsigned int frameStart = SDL_GetTicks();

  SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(m_renderer);

  SDL_RenderPresent(m_renderer);

  unsigned int frameTime = SDL_GetTicks() - frameStart;

  if (k_frameDelay > frameTime) {
    // std::cout << frameTime << std::endl;
    SDL_Delay(k_frameDelay - frameTime);
  }
}