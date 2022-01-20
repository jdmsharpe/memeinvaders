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

            std::cout << "XTEST" << x << std::endl;
            std::cout << "YTEST" << y << std::endl;
            if (x >= 505 && x <= 863){
              if (y >= 586 && y <= 618) {
                //std::cout << "START" << std::endl;
                gameState = GameState::GAME_MODE_1;
              } else if (y >= 648 && y <= 681) {
                std::cout << "OPTIONS" << std::endl;
                //gameState = GameState::GAME_MODE_1;
              } else if (y >= 709 && y <= 743) {
                //std::cout << "HIGH SCORE" << std::endl;
                gameState = GameState::HIGH_SCORE;
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
    // Cascade states with common behavior for now
    case GameState::SETTINGS:
    case GameState::MAIN_MENU:
    case GameState::HIGH_SCORE:
      if (keyboardState[SDL_SCANCODE_RETURN]) {
        gameState = GameState::GAME_MODE_1;
      }
      if (keyboardState[SDL_SCANCODE_H]) {
        gameState = GameState::HIGH_SCORE;
      }
      if (keyboardState[SDL_SCANCODE_R]) {
        gameState = GameState::MAIN_MENU;
      }
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
        window->ResetGameMode1();
        gameState = GameState::MAIN_MENU;
      }
      break;
    }
    window->Render(gameState);
  }

  window->Close();

  return 0;
}
