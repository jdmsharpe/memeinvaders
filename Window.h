#ifndef WINDOW_H
#define WINDOW_H

#include <vector>

#include "Player.h"

class Window {
public:
  Window();
  ~Window(){};

  // Create window, surface, and renderer
  bool Open();
  // Destroy window, surface, and renderer
  void Close();
  // Update entity positions and process commands
  void Render();

  inline void MovePlayer(SDL_Event *e) {
      m_player->Move(e);
  }

private:
  // The window we'll be rendering to
  SDL_Window *m_window = NULL;

  // The surface contained by the window
  SDL_Surface *m_screenSurface = NULL;

  // The renderer
  SDL_Renderer *m_renderer = NULL;

  // The player
  Player *m_player = nullptr;
};

#endif // WINDOW_H
