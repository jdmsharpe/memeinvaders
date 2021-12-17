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
  // Game starts on main menu
  GameState gameState = GameState::MAIN_MENU;
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

    // Change between game states on player input
    switch (gameState) {
    case GameState::MAIN_MENU:
      // Space to exit for now
      if (keyboardState[SDL_SCANCODE_SPACE]) {
        gameState = GameState::GAME_MODE_1;
      }
      break;

    case GameState::SETTINGS:
      break;

    case GameState::GAME_MODE_1:
      // Arrow keys to move
      if (keyboardState[SDL_SCANCODE_UP] || keyboardState[SDL_SCANCODE_DOWN] ||
          keyboardState[SDL_SCANCODE_LEFT] || keyboardState[SDL_SCANCODE_RIGHT]) {
        window->PlayerMoves(keyboardState);
      }
      window->EnemyMoves();
      // Space to fire
      if (keyboardState[SDL_SCANCODE_SPACE]) {
        window->PlayerFires();
      }
      break;
    }
    window->Render(gameState);
  }

  window->Close();

  return 0;
}
