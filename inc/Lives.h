#ifndef LIVES_H
#define LIVES_H

#include "AbstractEntity.h"

// Entity rendered to display player's life count.
class Lives : public AbstractEntity {
public:
  Lives(SDL_Renderer *renderer, int startingX, int startingY);
  ~Lives();

  // Grab image and create texture
  bool Initialize() override;
  // Render entity
  void Render() override;
};

#endif // LIVES_H