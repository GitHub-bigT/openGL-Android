#ifndef	_CAMERA_H_
#define _CAMERA_H_

#include <../GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>



enum  Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

//默认值
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;

const GLfloat SPEED = 20.0f;
const GLfloat SENSITIVTY = 0.15f;
const GLfloat ZOOM = 45.0F;

class Camera {
public:
	glm::vec3 Position;
	glm::vec3 WorldUp;

	glm::vec3 Up;
	glm::vec3 Front;
	glm::vec3 Rigth;
	//欧拉角
	GLfloat Yaw;
	GLfloat Pitch;
	GLfloat yaw = 90.0f;
	GLfloat pitch;

	//选项
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

	GLfloat radius = 0.0f;

	//构造函数。初始化列表
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : WorldUp(0.0f, 1.0f, 0.0f), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM){
		//this->Position = position;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->radius = position.z;
		this->updateCameraVectors();
		this->caluPosition();
	}
	void caluPosition(){
		glm::vec3 pos;
		pos.x = cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
		pos.y = sin(glm::radians(this->pitch));
		pos.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));
		this->Position = glm::normalize(pos) * radius;
	}

	//返回视图变换矩阵
	glm::mat4 getViewMatrix(){
		//return glm::lookAt(this->Position, this->Position + , this->Up);
		return glm::lookAt(this->Position, glm::vec3(0.0f,0.0f,0.0f), this->Up);
	}

	//键盘事件
	void ProcessKeyBoard(Camera_Movement direction, GLfloat deltaTime){
		GLfloat velocity = this->MovementSpeed * deltaTime;
		
		radius = sqrt(Position.x * Position.x + Position.y * Position.y + Position.z * Position.z);

		if (direction == FORWARD) 
		{
			//23 - 75
			if (radius <= 23.0f)
			{
				radius = 23.0f;
			}
			else
			{
				radius -= 1.0f * velocity;
			}
		}
		if (direction == BACKWARD)
		{
			if (radius >= 75.0f)
			{
				radius = 75.0f;
			}
			else
			{
				radius += 1.0f * velocity;
			}
		}

		this->caluPosition();

	}

	//鼠标移动事件
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset){

		this->yaw = xoffset;
		this->pitch = yoffset;
		this->caluPosition();
		
	}

	//用欧拉角计算摄像机向量
	void updateCameraVectors(){
		glm::vec3 front;
		front.x = cos(glm::radians(this->Pitch)) * cos(glm::radians(this->Yaw));
		front.y = sin(glm::radians(this->Pitch));
		front.z = cos(glm::radians(this->Pitch)) * sin(glm::radians(this->Yaw));

		this->Front = glm::normalize(front);
		this->Rigth = glm::normalize(glm::cross(this->WorldUp, this->Front));
		this->Up = glm::normalize(glm::cross(this->Rigth, this->Front));

	}
private:

};

#endif
