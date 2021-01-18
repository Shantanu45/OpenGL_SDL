#include "GameLoop.h"
#include "GL/glew.h"
#include <iostream>
#include <string>

void GameLoop::start(GameState& _game_state, SDL_Window* &window, SDL_Event(*poll)(GameState& state))
{
	while(_game_state != GameState::EXIT)
	{
		auto evnt = poll(_game_state);

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SDL_GL_SwapWindow(window);
	}
}
