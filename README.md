# memeinvaders
Space invaders clone in C++
Fonts used:
-Silkscreen by Jason Kottke

To build:

1. Install SDL2 (and possibly OpenGL): https://wiki.libsdl.org/Installation
2. Install CMake: https://cmake.org/install/
3. Create a build folder at the same level as the repo, i.e. `mkdir memeinvaders_build`
4. `cd memeinvaders_build && cmake ../memeinvaders`
5. `cmake --build .`
6. Finally, run the executable with: `./MemeInvaders`

To debug (VSCode only):

1. Ensure build is built with debug flags in CMake file
2. Build as usual (steps above)
3. Edit `launch_template.json` under `.vscode` as directed in the file
4. Rename `launch_template.json` to `launch.json`
5. Use VSCode's Run and Debug option in the left pane to debug the application
