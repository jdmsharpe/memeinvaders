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
      }
    }

    // Get all keys currently pressed
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);

    // Player presses arrow keys to move
    if (keyboardState[SDL_SCANCODE_UP] || keyboardState[SDL_SCANCODE_DOWN] ||
        keyboardState[SDL_SCANCODE_LEFT] || keyboardState[SDL_SCANCODE_RIGHT]) {
      window->PlayerMoves(keyboardState);
    }

    // Player presses space to fire
    if (keyboardState[SDL_SCANCODE_SPACE]) {
      window->PlayerFires();
    }

    // Update window
    window->Render();
  }

  window->Close();

  return 0;
}