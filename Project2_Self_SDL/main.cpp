#include <Windows.h>
#include <iostream>
#include <GL/glew.h>
#include <SDL/SDL.h>

enum class GameState {PLAY, EXIT};

int main(int argc, char** argv)
{
	if(SDL_Init(SDL_INIT_EVERYTHING))
	{
		std::cout << "SDL Can't Initialize";
		return 1;
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Window* mainWindow = SDL_CreateWindow("Game",
									SDL_WINDOWPOS_CENTERED,
									SDL_WINDOWPOS_CENTERED,
									1024, 768, SDL_WINDOW_OPENGL);

	SDL_GLContext glContext = SDL_GL_CreateContext(mainWindow);

	glewExperimental = GL_TRUE;

	if(glewInit() != GLEW_OK)
	{
		std::cout << "unable to initialize GLEW";
		SDL_DestroyWindow(mainWindow);
		SDL_Quit();
	}
	glViewport(0, 0, 1024, 768);

	GameState _game_state = GameState::PLAY;


	while(_game_state != GameState::EXIT)
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
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SDL_GL_SwapWindow(mainWindow);
	}
	return 0;
}