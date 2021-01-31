#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed):
	position(startPosition), 
	worldUp(startUp), yaw(startYaw), pitch(startPitch), 
	front(glm::vec3(0.0f, 0.0f, -1.0f)),
	moveSpeed(startMoveSpeed), turnSpeed(startTurnSpeed),
	w_pressed(false), a_pressed(false), s_pressed(false), d_pressed(false)
{
	update();
}

void Camera::keyControl(SDL_Event& evnt, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;
	if(evnt.type == SDL_KEYDOWN)
	{
		std::cout << "PRESS" << std::endl;
		switch( evnt.key.keysym.sym )
		{
			case SDLK_w:
				w_pressed = true;
				break;
			case SDLK_s:
				s_pressed = true;
				break;
			case SDLK_a:
				a_pressed = true;
				break;
			case SDLK_d:
				d_pressed = true;
				break;
			default: break;
		}
	}
	else if(evnt.type == SDL_KEYUP)
	{
		std::cout << "RELEASE" << std::endl;
		switch( evnt.key.keysym.sym )
		{
			case SDLK_w:
				w_pressed = false;
				break;
			case SDLK_s:
				s_pressed = false;
				break;
			case SDLK_a:
				a_pressed = false;
				break;
			case SDLK_d:
				d_pressed = false;
				break;
			default: break;
		}
	}

	if (w_pressed)
		position += front * velocity;
	if (s_pressed)
		position -= front * velocity;
	if (a_pressed)
		position -= right * velocity;
	if (d_pressed)
		position += right * velocity;
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if(pitch > 89.0f)
	{
		pitch = 89.0;
	}

	if(pitch < -89.0)
	{
		pitch = -89.0;
	}

	update();
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}


void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);			// make front a unit vector

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera()
{
}