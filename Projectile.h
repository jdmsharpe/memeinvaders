#ifndef PROJECTILE_H
#define PROJECTILE_H

class Projectile {
public:
  Projectile(bool shouldBeRendered, int startingX, int startingY);
  ~Projectile(){};

  void Render();

private:
  bool m_shouldBeRendered = false;

  int m_xPos, m_yPos;
};

#endif // PROJECTILE_H