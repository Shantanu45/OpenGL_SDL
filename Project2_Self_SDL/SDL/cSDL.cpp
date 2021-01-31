#include "cSDL.h"
#include <iostream>
#include <GL/glew.h>


cSDL::cSDL(): relX(0), relY(0), mouseMove(false)
{
}

int cSDL::initialize(SDL_Window* &window, int w, int h)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "SDL Can't Initialize: " << SDL_GetError();
		return -1;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	glewExperimental = GL_TRUE;

	window = SDL_CreateWindow("Game",
									SDL_WINDOWPOS_CENTERED,
									SDL_WINDOWPOS_CENTERED,
									w, h, SDL_WINDOW_OPENGL);
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	return 0;
}

SDL_Event cSDL::poll(GameState& _game_state)
{
	mouseMove = false;
	relX = 0;
	relY = 0;
	SDL_Event evnt;
	while(SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			_game_state = GameState::EXIT;
			break;

		case SDL_MOUSEMOTION:
			mouseMove = true;
			relX = evnt.motion.xrel;
			relY = evnt.motion.yrel;
			break;

		case SDL_KEYDOWN:
			switch( evnt.key.keysym.sym ){
				case SDLK_ESCAPE:
					_game_state = GameState::EXIT;
					break;
				default: 
					return evnt;
			}

		default: 
			break;
		}
	}
	return evnt;
}

GLint cSDL::getRelX()
{
	return relX;
}

GLint cSDL::getRelY()
{
	return relY;
}

