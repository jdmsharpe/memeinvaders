#ifndef ABSTRACT_ENTITY_H
#define ABSTRACT_ENTITY_H

#include <memory>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <string>
#include <utility>

#include "Defs.h"
#include "Macros.h"

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
  }

  std::string GetName() { return m_name; }

  // Grab image and create texture
  virtual bool Initialize() = 0;
  // Render entity
  virtual void Render() = 0;

  bool ImgInit() {
    // Something really went wrong
    if (m_renderer == NULL) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                  "MainMenu pointer to renderer was NULL!");
      return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Image could not initialize!");
      return false;
    }

    return true;
  }

  bool CreateTexture(const std::string& filename) {
    // Load image and create texture
    SDL_Surface *surface = IMG_Load(filename.c_str());
    if (surface == NULL) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                  "Unable to load image %s! Error code: %s.", filename.c_str(),
                  SDL_GetError());
      return false;
    }

    m_texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_FreeSurface(surface);
    return true;
  }

protected:
  SDL_Renderer *m_renderer = NULL;
  SDL_Texture *m_texture = NULL;

  SDL_Rect *m_screenBox = NULL;
  SDL_Rect *m_textureBox = NULL;

  std::string m_name;
};

#endif // ABSTRACT_ENTITY_H