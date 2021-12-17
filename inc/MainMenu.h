#ifndef MAINMENU_H
#define MAINMENU_H

#include "AbstractEntity.h"

// Entity rendered when the current game state is MAINMENU.
class MainMenu : public AbstractEntity {
public:
  MainMenu(SDL_Renderer *renderer);
  ~MainMenu() {};

  // Grab image and create texture
  bool Initialize() override { return true; };
  // Render entity
  void Render() override {};

private:

};

#endif // MAINMENU_H