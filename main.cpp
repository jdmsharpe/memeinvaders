#include <iostream>
#include <memory>

#include "Window.h"

int main(int argc, char *argv[]) {
  // Construct SDL window
  std::unique_ptr<Window> window = std::make_unique<Window>();
  window->Open();

  // Main loop flag
  bool quit = false;
  // Event handler
  SDL_Event e;

  while (!quit) {
    // Handle queue
    while (SDL_PollEvent(&e) != 0) {
      // User closes window
      if (e.type == SDL_QUIT) {
        quit = true;
      }
    }

    // Update window
    window->Render();
  }

  window->Close();

  return 0;
}