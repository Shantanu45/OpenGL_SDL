#pragma once
#include <SDL/SDL.h>
#include "../GameState.h"
#include "GL\glew.h"

class cSDL
{
public:
	cSDL();
	~cSDL() = default;

	int initialize(SDL_Window* &window, int w, int h);
	SDL_Event poll(GameState& _game_state);

	GLint getRelY();
	GLint getRelX();

private:
	GLint relX, relY;
	bool mouseMove;
};
