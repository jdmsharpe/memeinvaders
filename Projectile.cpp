#include "Projectile.h"

Projectile::Projectile(bool shouldBeRendered, int startingX, int startingY)
      : m_shouldBeRendered(shouldBeRendered), m_xPos(startingX),
        m_yPos(startingY) {}

void Projectile::Render() {
    if (m_shouldBeRendered) {
        
    }
}