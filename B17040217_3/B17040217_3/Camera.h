#pragma once

#define GLEW_STATIC
#include<GL/glew.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include<vector>

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

//�������
const GLfloat YAW = -90.0f;//������
const GLfloat PITCH = 0.0f;//���ҷ���
const GLfloat SPEED = 6.0f;
const GLfloat SENSITIVITY = 0.25f;
const GLfloat ZOOM = 45.0f;//����

class Camera {
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) :front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {//�ṩ�����Ĺ��캯��
		this->position = position;
		this->worldup = up;
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	Camera(GLfloat posX, GLfloat posY, GLfloat posZ,GLfloat upX,GLfloat upY,GLfloat upZ, GLfloat yaw = YAW, GLfloat pitch = PITCH) :front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {//�ṩ�����Ĺ��캯��
		this->position = glm::vec3(posX,posY,posZ);
		this->worldup = glm::vec3(upX,upY,upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}
	void ProcessKeyboard(Camera_Movement direction,GLfloat deltaTime) {
		//������̲���
		GLfloat velocity = this->movementSpeed *deltaTime;
		if (direction == FORWARD) {
			this->position += this->front*velocity;
		}
		if (direction == BACKWARD) {
			this->position -= this->front*velocity;
		}
		if (direction == LEFT) {
			this->position += this->right*velocity;
		}
		if (direction == RIGHT) {
			this->position -= this->right*velocity;
		}
	}
	void ProcessMouseMovement(GLfloat  xOFFset,GLfloat yOFFset,GLboolean constrainPitch=true) {
		//����������
		xOFFset *= this->mouseSensitivity;
		yOFFset *= this->mouseSensitivity;
		this->pitch += yOFFset;
		this->yaw -= xOFFset;

		if (constrainPitch) {
			if (this->pitch > 89.0f) {
				this->pitch = 89.0f;
			}
			if (this->pitch < -89.0f){
				this->pitch = -89.0f;
			}
		}

		updateCameraVectors();
	}

	glm::mat4 GetViewMatrix() {
		return
			glm::lookAt(this->position, this->position + this->front, this->up);
	}

	GLfloat getZoom() {
		return this->zoom;
	}
	glm::vec3 getPosition() {
		return this->position;
	}
private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 worldup;
	GLfloat yaw;
	GLfloat pitch;
	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
	GLfloat zoom;
	void updateCameraVectors() {
		glm::vec3 front;
		front.x = cos(glm::radians(this->pitch))*cos(glm::radians(this->yaw));
		front.y = sin(glm::radians(this->pitch));
		front.z = cos(glm::radians(this->pitch))*sin(glm::radians(this->yaw));
		this->front = glm::normalize(front);
		this->right = glm::normalize(glm::cross(this->front, this->worldup));
		this->up = glm::normalize(glm::cross(this->right, this->front));

	}
};