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
    while (SDL_PollEvent(&e)) {
      // User closes window
      switch(e.type) {
        case SDL_QUIT:
          quit = true;
          break;
        case SDL_MOUSEBUTTONDOWN:
          const SDL_MouseButtonEvent &mouse_event = e.button;
          int x,y;

          if (mouse_event.button == SDL_BUTTON_LEFT && gameState == GameState::MAIN_MENU) {
            SDL_GetMouseState(&x, &y);

            //std::cout << "XTEST" << x << std::endl;
            //std::cout << "YTEST" << y << std::endl;
            if (x >= 560 && x <= 810){
              if (y >= 610 && y <= 655) {
                //std::cout << "START" << std::endl;
                gameState = GameState::GAME_MODE_1;
              }
            }
          }

      }
    }

    // Get all keys currently pressed
    const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
    // Get mouseclick events
    //const Uint8 *mouseEvent = SDL_MouseButtonEvent(NULL);

    // Pseudo-state machine to handle switching between game states
    // Should probably be cleaned up at some point (or just not be in main)
    switch (gameState) {
    case GameState::MAIN_MENU:
      // Enter to exit for now
      if (keyboardState[SDL_SCANCODE_RETURN]) {
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
      for (int i = 0; i < window->m_startingNumEnemies; ++i) {
        window->EnemyFires(i);
        window->EnemyMoves(i);
      }
      // Space to fire
      if (keyboardState[SDL_SCANCODE_SPACE]) {
        window->PlayerFires();
      }
      // R to reset game
      if (keyboardState[SDL_SCANCODE_R]) {
        //window->ResetGameMode1();
        gameState = GameState::MAIN_MENU;
      }
      break;
    }
    window->Render(gameState);
  }

  window->Close();

  return 0;
}
