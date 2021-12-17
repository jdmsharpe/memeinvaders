#ifndef ENTITY_H
#define ENTITY_H
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <string>
#include <utility>

#include "Defs.h"

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

  inline std::pair<double, double> GetVelocity() {
    return std::make_pair(m_xVel, m_yVel);
  }

  // Grab image and create texture
  virtual bool Initialize() = 0;
  // Render entity
  virtual void Render() = 0;

protected:
  inline void SetPosition(int xPos, int yPos) {
    m_xPos = xPos;
    m_yPos = yPos;
  }

  inline void SetVelocity(double xVel, double yVel) {
    m_xVel = xVel;
    m_yVel = yVel;
  }

  inline void UpdatePositionFromVelocity() {
    double xPosResult = m_xVel * DELTA_TIME;
    double yPosResult = m_yVel * DELTA_TIME;
    m_xPos += round(xPosResult);
    m_yPos += round(yPosResult);
  }

  // Velocities can be double but position is cast to int to be passed into
  // SDL_Rect
  int m_xPos, m_yPos;
  double m_xVel, m_yVel;

  SDL_Renderer *m_renderer = NULL;
  SDL_Texture *m_texture = NULL;

  SDL_Rect *m_screenBox = NULL;
  SDL_Rect *m_textureBox = NULL;
};

#endif // ENTITY_H
