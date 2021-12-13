#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "ScreenDefs.h"

class Player {
public:
  Player(SDL_Renderer *renderer);
  ~Player();

  std::pair<int, int> GetPosition() { return std::make_pair(m_xPos, m_yPos); }

  // Grab image and create texture
  bool Initialize();
  // Render entity
  void Render();
  // Process keyboard input
  void Move(SDL_Event *e);
  void Fire();

private:
  SDL_Renderer *m_renderer = NULL;
  SDL_Texture *m_texture = NULL;

  SDL_Rect *m_screenBox = NULL;
  SDL_Rect *m_textureBox = NULL;

  int m_xPos, m_yPos;

  // Keeps track of shots on screen
  int m_shotsPresent = 0;
};

#endif // PLAYER_H
