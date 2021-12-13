#include <iostream>
#include <memory>

#include "Window.h"

int main(int argc, char *argv[]) {
  // Construct SDL window
  std::unique_ptr<Window> window = std::make_unique<Window>();
  if (!window->Open()) {
      // Failed :(
      SDL_Delay(500);
      return -1;
  }

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
      } else if (e.type == SDL_KEYDOWN) {
        std::cout << "Got a keypress!" << std::endl;
        window->MovePlayer(&e);
      }
    }

    // Update window
    window->Render();
  }

  window->Close();

  return 0;
}