#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>

class Window 
{
public:
    Window();
    ~Window() {};

    bool Open();

    void Close();

    void Render();

private:
	// The window we'll be rendering to
	SDL_Window* m_window = NULL;

	// The surface contained by the window
	SDL_Surface* m_screenSurface = NULL;

	// The renderer
	SDL_Renderer* m_renderer = NULL;
};

#endif // WINDOW_H
