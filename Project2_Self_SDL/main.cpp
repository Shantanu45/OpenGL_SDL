#include <Windows.h>
#include <iostream>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <fstream>

#include "GameState.h"
#include "SDL/cSDL.h"
#include "GLEW/cGLEW.h"
#include <string>

const GLint width = 1024;
const GLint height = 768;

GLuint VAO, VBO, shader;

void ReadShaders(const char* filePath, std::string& target)
{
	std::ifstream stream;

	stream.open(filePath);

	if (stream.is_open())
	{
		std::string line;
		while(!stream.eof())
		{
			std::getline(stream, line);
			target += line + "\n";
		}
	}
}

void CreateTriangle()
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f
	};
	
	glGenVertexArrays(1, &VAO);		// Vertex Array Object
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);			// Vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) 
	{
		glGetShaderInfoLog(theShader, 1024, NULL, eLog);
		fprintf(stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}

void CompileShaders(std::string vShader, std::string fShader)
{
	shader = glCreateProgram();

	if (!shader) 
	{
		printf("Failed to create shader\n");
		return;
	}

	AddShader(shader, vShader.c_str(), GL_VERTEX_SHADER);
	AddShader(shader, fShader.c_str(), GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result) 
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result) 
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

}

int main(int argc, char** argv)
{
	SDL_Window* mainWindow = nullptr;
	
	auto _game_state = GameState::PLAY;
	cSDL* sdl = new cSDL();
	cGLEW* glew = new cGLEW();
	
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

	std::string vShader = "";
	std::string fShader = "";
	ReadShaders("Shaders/shader.vert", vShader);
	ReadShaders("Shaders/shader.frag", fShader);

	CreateTriangle();
	CompileShaders(vShader, fShader);

	while(_game_state != GameState::EXIT)
	{
		auto evnt = sdl->poll(_game_state);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader);

		glBindVertexArray(VAO);				// because of VAO, here we don't have to bind VBO and attribarray
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		
		glUseProgram(0);

		SDL_GL_SwapWindow(mainWindow);
	}
	
	return EXIT_SUCCESS;
}