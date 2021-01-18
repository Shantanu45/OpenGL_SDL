#include "cGLEW.h"

#include <GL/glew.h>
#include <iostream>

int cGLEW::initialize(SDL_Window*& window, int w, int h)
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
