#pragma once

#define STB_IMAGE_IMPLEMENTATION

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
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
