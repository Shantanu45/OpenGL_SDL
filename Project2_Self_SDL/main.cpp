#include <Windows.h>
#include <iostream>
#include <SDL/SDL.h>

#include "GameState.h"
#include "SDL/cSDL.h"
#include "GL/cGLEW.h"
#include "Game/GameLoop.h"

int main(int argc, char** argv)
{
	const int width = 1024;
	const int height = 768;
	SDL_Window* mainWindow = nullptr;
	auto _game_state = GameState::PLAY;
	cSDL* sdl = new cSDL();
	cGLEW* glew = new cGLEW();
	GameLoop* gameLoop = new GameLoop();
	// Initialize SDL
	if(sdl->initialize(mainWindow, width, height) == -1)
		return EXIT_FAILURE;

	// Initialize GLEW 
	if(glew->initialize(mainWindow, width, height))
	{
		std::cout << "cannot initialize GLEW or main window in null!";
		SDL_Quit();
		return EXIT_FAILURE;
	}
	
	// main game loop
	gameLoop->start(_game_state, mainWindow);
	
	return EXIT_SUCCESS;
}