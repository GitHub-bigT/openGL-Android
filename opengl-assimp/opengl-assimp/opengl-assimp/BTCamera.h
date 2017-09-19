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

//Ĭ��ֵ
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
	//ŷ����
	GLfloat Yaw;
	GLfloat Pitch;

	//ѡ��
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

	GLfloat radius = 0.0f;

	//���캯������ʼ���б�
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : WorldUp(0.0f, 1.0f, 0.0f), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM){
		this->Position = position;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->radius = position.z;
		this->updateCameraVectors();
	}

	//������ͼ�任����
	glm::mat4 getViewMatrix(){
		//return glm::lookAt(this->Position, this->Position + , this->Up);
		return glm::lookAt(this->Position, glm::vec3(0.0f,0.0f,0.0f), this->Up);
	}

	//�����¼�
	void ProcessKeyBoard(Camera_Movement direction, GLfloat deltaTime){
		GLfloat velocity = this->MovementSpeed * deltaTime;
		if (direction == FORWARD)
		{
			this->Position += this->Front * velocity;
			radius += 1.0f * velocity;
		}
		if (direction == BACKWARD)
		{
			this->Position -= this->Front * velocity;
			radius -= 1.0f * velocity;
		}
		if (direction == LEFT)
		{
			this->Position -= this->Rigth * velocity;
		}
		if (direction == RIGHT)
		{
			this->Position += this->Rigth * velocity;
		}
		//FPS�����  ֻ����xyƽ����
		//this->Position.y = 0.0f;
	}

	//����ƶ��¼�
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset){
		//xoffset *= this->MouseSensitivity;
		//yoffset *= this->MouseSensitivity;
		//��ʱ����תΪ����ƫ���Ǻ͸�����Ӧ��-=ƫ����
		this->Yaw = xoffset;
		this->Pitch = yoffset;

		//printf("ƫ���ǣ�%f�������ǣ�%f\n", this->Yaw, this->Pitch);

		if (this->Pitch >= 89.0f)
		{
			this->Pitch = 89.0f;
		}
		if (this->Pitch <= -89.0f)
		{
			this->Pitch = -89.0f;
		}

		//float x = cos(glm::radians(this->Pitch)) * cos(glm::radians(this->Yaw));
		//float y = sin(glm::radians(this->Pitch));
		//float z = cos(glm::radians(this->Pitch)) * sin(glm::radians(this->Yaw));
		//glm::vec3 mm = glm::vec3(x,y,z);
		printf("������뾶��%f\n",radius);
		this->updateCameraVectors();
		this->Position = this->Front * radius;
	}

	//�����ַŴ���С
	void ProcessMouseScroll(GLfloat yoffset){
		if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
		{
			this->Zoom -= yoffset;
		}
		if (this->Zoom >= 45.0f)
		{
			this->Zoom = 45.0f;
		}
		if (this->Zoom <= 1.0f)
		{
			this->Zoom = 1.0f;
		}
	}
	//��ŷ���Ǽ������������
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
