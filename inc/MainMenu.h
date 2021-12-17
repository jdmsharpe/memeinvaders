#ifndef MAINMENU_H
#define MAINMENU_H

class MainMenu : public Page {
public:
  MainMenu(SDL_Renderer *renderer);
  ~MainMenu();

  // Grab image and create texture
  bool Initialize() override;
  // Render entity
  void Render() override;

private:
}

#endif // MAINMENU_H