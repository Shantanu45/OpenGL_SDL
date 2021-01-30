#include <Windows.h>
#include <iostream>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

#include "GameState.h"
#include "SDL/cSDL.h"
#include "GLEW/cGLEW.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "Mesh.h"

const GLint width = 1024;
const GLint height = 768;

std::vector<Mesh*> meshList;

GLuint shader, uniformModel, uniformProjection;

bool direction = true;
float triOffset = 0.0f;
float triMaxoffset = 0.7f;
float triIncrement = 0.0005f;

// Declarations
void ReadShaders(const char* filePath, std::string& target);
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
void CompileShaders(std::string vShader, std::string fShader);
void CreateTriangle();

//main function
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

	glEnable(GL_DEPTH_TEST);

	std::string vShader = "";
	std::string fShader = "";
	ReadShaders("Shaders/shader.vert", vShader);
	ReadShaders("Shaders/shader.frag", fShader);

	CreateTriangle();
	CompileShaders(vShader, fShader);

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);

	while(_game_state != GameState::EXIT)
	{
		auto evnt = sdl->poll(_game_state);

		if(direction)
		{
			triOffset += triIncrement;
		}
		else{
			triOffset -= triIncrement;
		}

		//if(abs(triOffset) >= triMaxoffset)
		//{
		//	direction = !direction;
		//}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::rotate(model, triOffset ,glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		meshList[0]->RenderMesh();
		
		glUseProgram(0);

		SDL_GL_SwapWindow(mainWindow);
	}
	
	return EXIT_SUCCESS;
}

void ReadShaders(const char* filePath, std::string& target)
{
	std::ifstream stream;

	stream.open(filePath);

	if (stream.is_open())
	{
		std::string line;
		while (!stream.eof())
		{
			std::getline(stream, line);
			target += line + "\n";
		}
	}
}

void CreateTriangle()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, -0.5f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -0.5f,
		0.0f, 1.0f, 0.0f
	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.emplace_back(obj1);

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

	uniformModel = glGetUniformLocation(shader, "model");	// returns id of 'xMove uniform from shader.vert'
	uniformProjection = glGetUniformLocation(shader, "projection");	// returns id of 'xMove uniform from shader.vert'
}