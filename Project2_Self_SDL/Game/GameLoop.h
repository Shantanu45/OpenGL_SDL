#pragma once
#include "../GameState.h"
#include "SDL/SDL.h"
class GameLoop
{
public:
	GameLoop() = default;
	~GameLoop() = default;

	SDL_Event SDL_POLL(GameState& _game_state);
	void start(GameState& _game_state, SDL_Window* &window);
};
