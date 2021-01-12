#pragma once
#include <SDL/SDL.h>
#include "../GameState.h"


class cSDL
{
public:
	cSDL() = default;
	~cSDL() = default;

	int initialize(SDL_Window* &window, int w, int h);
	static SDL_Event poll(GameState& _game_state);
};
