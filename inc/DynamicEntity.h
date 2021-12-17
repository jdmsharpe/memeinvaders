#ifndef DYNAMIC_ENTITY_H
#define DYNAMIC_ENTITY_H

#include "AbstractEntity.h"

// Abstract class for in-game entities that move and have
// positions and velocities.
class DynamicEntity : public AbstractEntity {
public:
  DynamicEntity(SDL_Renderer *renderer) : AbstractEntity(renderer) {}

  ~DynamicEntity() {}

  inline std::pair<int, int> GetPosition() {
    return std::make_pair(m_xPos, m_yPos);
  }

  inline std::pair<double, double> GetVelocity() {
    return std::make_pair(m_xVel, m_yVel);
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

  // Velocities can be double but position is cast to int to be passed into
  // SDL_Rect
  int m_xPos, m_yPos;
  double m_xVel, m_yVel;
};

#endif // DYNAMIC_ENTITY_H
