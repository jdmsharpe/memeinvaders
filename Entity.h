#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <utility>

// Abstract class that all objects in game inherit from
class Entity {
public:
  Entity(SDL_Renderer *renderer) : m_renderer(renderer) {}

  ~Entity() {
    // Clean up boxes
    delete m_screenBox;
    m_screenBox = NULL;

    delete m_textureBox;
    m_textureBox = NULL;

    // Clean up texture
    SDL_DestroyTexture(m_texture);
    m_texture = NULL;
  };

  inline std::pair<int, int> GetPosition() {
    return std::make_pair(m_xPos, m_yPos);
  }

  // Grab image and create texture
  virtual bool Initialize() = 0;
  // Render entity
  virtual void Render() = 0;

protected:
  inline void SetPosition(std::pair<int, int> pos) {
    m_xPos = pos.first;
    m_yPos = pos.second;
  }

  SDL_Renderer *m_renderer = NULL;
  SDL_Texture *m_texture = NULL;

  SDL_Rect *m_screenBox = NULL;
  SDL_Rect *m_textureBox = NULL;

  int m_xPos, m_yPos;
};

#endif // ENTITY_H