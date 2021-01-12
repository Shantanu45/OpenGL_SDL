#include <Windows.h>
#include <iostream>
#include <GL/glew.h>
#include <SDL/SDL.h>

enum class GameState {PLAY, EXIT};

int initialize_SDL(SDL_Window* &window, int w, int h)
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

	window = SDL_CreateWindow("Game",
									SDL_WINDOWPOS_CENTERED,
									SDL_WINDOWPOS_CENTERED,
									w, h, SDL_WINDOW_OPENGL);
	return 0;
}

int initialize_GLEWGL(SDL_Window* &window, int w, int h)
{
	if(window == nullptr)
	{
		std::cout << "Window could not be created: " << SDL_GetError() << std::endl;
		//Quit SDL subsystems
		SDL_Quit();
		return -1;
	}

	auto *gl_context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;

	if(glewInit() != GLEW_OK)
	{
		std::cout << "unable to initialize GLEW";
		//Destroy window
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}
	glViewport(0, 0, w, h);

	return 0;
}

SDL_Event SDL_POLL(GameState& _game_state)
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

void game_loop(GameState& _game_state, SDL_Window* &window)
{
	while(_game_state != GameState::EXIT)
	{
		SDL_Event evnt = SDL_POLL(_game_state);

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		SDL_GL_SwapWindow(window);
	}
}

int main(int argc, char** argv)
{
	const int width = 1024;
	const int height = 768;
	SDL_Window* mainWindow = nullptr;
	
	if(initialize_SDL(mainWindow, width, height) == -1)
		return EXIT_FAILURE;
	
	if(initialize_GLEWGL(mainWindow, width, height))
	{
		std::cout << "cannot initialize GLEW or main window in null!";
		SDL_Quit();
		return EXIT_FAILURE;
	}
	
	auto _game_state = GameState::PLAY;

	game_loop(_game_state, mainWindow);
	
	return EXIT_SUCCESS;
}