#ifndef MAINMENUOPTIONS_H
#define MAINMENUOPTIONS_H

#include <iostream>

#include "AbstractEntity.h"

// Entity rendered when the current game state is MAINMENU.
class MainMenuOptions : public AbstractEntity {
public:
  MainMenuOptions(SDL_Renderer *renderer);
  ~MainMenuOptions();

  // Grab image and create texture
  bool Initialize() override;
  // Render entity
  void Render() override;
};

#endif // MAINMENU_H
