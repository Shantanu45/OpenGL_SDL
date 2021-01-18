#pragma once
#include <SDL/SDL.h>

class cGLEW
{
public:
	cGLEW() = default;
	~cGLEW() = default;

	int initialize(SDL_Window* &window, int w, int h);
};
