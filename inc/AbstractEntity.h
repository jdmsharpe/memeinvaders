#ifndef ABSTRACT_ENTITY_H
#define ABSTRACT_ENTITY_H

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <string>
#include <utility>

#include "Defs.h"

// Abstract class that all objects in game inherit from.
// Has associated texture to render sprite to screen.
class AbstractEntity {
public:
  AbstractEntity(SDL_Renderer *renderer, std::string name)
      : m_renderer(renderer), m_name(name) {}

  ~AbstractEntity() {
    // Clean up boxes
    delete m_screenBox;
    m_screenBox = NULL;

    delete m_textureBox;
    m_textureBox = NULL;

    // Clean up texture
    SDL_DestroyTexture(m_texture);
    m_texture = NULL;
  };

  std::string GetName() { return m_name; }

  // Grab image and create texture
  virtual bool Initialize() = 0;
  // Render entity
  virtual void Render() = 0;

protected:
  SDL_Renderer *m_renderer = NULL;
  SDL_Texture *m_texture = NULL;

  SDL_Rect *m_screenBox = NULL;
  SDL_Rect *m_textureBox = NULL;

  std::string m_name;
};

#endif // ABSTRACT_ENTITY_H