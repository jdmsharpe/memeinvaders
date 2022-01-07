#ifndef DYNAMIC_ENTITY_H
#define DYNAMIC_ENTITY_H

#include "AbstractEntity.h"

// Abstract class for in-game entities that move and have
// positions and velocities.
class DynamicEntity : public AbstractEntity {
public:
  DynamicEntity(SDL_Renderer *renderer, std::string name, int width, int height)
      : AbstractEntity(renderer, name), m_width(width), m_height(height) {
    m_boundingBox =
        std::make_unique<BoundingBox>(m_xPos, m_yPos, m_width, m_height);
  }

  ~DynamicEntity() {}

  inline std::pair<int, int> GetPosition() {
    return std::make_pair(m_xPos, m_yPos);
  }

  inline std::pair<double, double> GetVelocity() {
    return std::make_pair(m_xVel, m_yVel);
  }

  inline BoundingBox* GetBoundingBox() {
    m_boundingBox->left = m_xPos;
    m_boundingBox->right = m_xPos + m_width;
    m_boundingBox->top = m_yPos;
    m_boundingBox->bottom = m_yPos + m_height;
    return m_boundingBox.get();
  }

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

  int m_width, m_height;
  std::unique_ptr<BoundingBox> m_boundingBox;

  // Velocities can be double but position is cast to int to be passed into
  // SDL_Rect
  int m_xPos, m_yPos;
  double m_xVel, m_yVel;
};

#endif // DYNAMIC_ENTITY_H
