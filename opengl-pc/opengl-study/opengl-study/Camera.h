#ifndef	_CAMERA_H_
#define _CAMERA_H_

#include <GL\glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Camera {
public:
	glm::vec3 Position;
	glm::vec3 WorldUp;
	glm::vec3 Up;
	glm::vec3 Front;
	glm::vec3 Rigth;
	//Å·À­½Ç
	GLfloat Yaw;
	GLfloat Pitch;
	//Ñ¡Ïî
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

	//
private:
};

#endif
