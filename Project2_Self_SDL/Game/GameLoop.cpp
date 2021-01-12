#include "GameLoop.h"
#include "GL/glew.h"

void GameLoop::start(GameState& _game_state, SDL_Window* &window)
{
	while(_game_state != GameState::EXIT)
	{
		SDL_POLL(_game_state);

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SDL_GL_SwapWindow(window);
	}
}

SDL_Event GameLoop::SDL_POLL(GameState& _game_state)
{
	SDL_Event evnt;
	while(SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			_game_state = GameState::EXIT;
			break;
		default: break;
		}
	}
	return evnt;
}
