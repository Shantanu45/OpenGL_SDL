#include "Includes.h"

const GLint width = 1024;
const GLint height = 768;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

std::string vShader = "Shaders/shader.vert";
std::string fShader = "Shaders/shader.frag";

// Declarations
void CreateObjects();
void CreateShaders();

//main function
int main(int argc, char** argv)
{
	SDL_Window* mainWindow = nullptr;
	
	auto _game_state = GameState::PLAY;
	
	// Initialize SDL
	cSDL* sdl = new cSDL();
	if(sdl->initialize(mainWindow, width, height) == -1)
		return EXIT_FAILURE;
	
	// Initialize GLEW 
	cGLEW* glew = new cGLEW();
	if(glew->initialize(mainWindow, width, height))
	{
		std::cout << "cannot initialize GLEW or main window in null!";
		SDL_Quit();
		return EXIT_FAILURE;
	}

	glEnable(GL_DEPTH_TEST);

	CreateObjects();
	CreateShaders();
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.001f, 0.2f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTexture();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTexture();

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);

	while(_game_state != GameState::EXIT)
	{
		GLfloat now = SDL_GetPerformanceCounter();
		deltaTime = (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;

		auto evnt = sdl->poll(_game_state);

		camera.keyControl(evnt, deltaTime);
		camera.mouseControl(sdl->getRelX(), sdl->getRelY());

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLoaction();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		brickTexture.UseTexture();

		meshList[0]->RenderMesh();
		
		glUseProgram(0);

		SDL_GL_SwapWindow(mainWindow);
	}
	
	return EXIT_SUCCESS;
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		-1.0f,  -1.0f, -0.5f,  0.0f, 0.0f,
		 0.0f,  -1.0f,  1.0f,  0.5f, 0.0f,
		 1.0f,  -1.0f, -0.5f,  1.0f, 0.0f,
		 0.0f,   1.0f,  0.0f,  0.5f, 1.0f
	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 20, 12);
	meshList.emplace_back(obj1);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader.c_str(), fShader.c_str());
	shaderList.emplace_back(*shader1);
}

//void ReadShaders(const char* filePath, std::string& target)
//{
//	std::ifstream stream;
//
//	stream.open(filePath);
//
//	if (stream.is_open())
//	{
//		std::string line;
//		while (!stream.eof())
//		{
//			std::getline(stream, line);
//			target += line + "\n";
//		}
//	}
//}