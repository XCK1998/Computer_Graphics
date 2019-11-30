#pragma once

#define GLEW_STATIC
#include<GL/glew.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

#include<vector>

enum Camera_Movement
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	SPACE
};

//定义相机
const GLfloat YAW = -90.0f;//俯仰角
const GLfloat PITCH = 0.0f;//左右幅度
const GLfloat SPEED = 6.0f;
const GLfloat SENSITIVITY = 0.25f;
const GLfloat ZOOM = 45.0f;//缩放

class Camera {
public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH):front(glm::vec3(0.0f, 1.0f, 0.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {//提供向量的构造函数
		this->position = position;
		this->initialPos = position;
		this->cameraPos = position;
		this->worldup = up;
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	Camera(GLfloat posX, GLfloat posY, GLfloat posZ,GLfloat upX,GLfloat upY,GLfloat upZ, GLfloat yaw = YAW, GLfloat pitch = PITCH) :front(glm::vec3(0.0f, 1.0f, 0.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {//提供标量的构造函数
		this->position = glm::vec3(posX,posY,posZ);
		this->cameraPos = glm::vec3(posX, posY, posZ);
		this->initialPos = position;
		this->worldup = glm::vec3(upX,upY,upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime) {
		//处理键盘操作
		GLfloat velocity = this->movementSpeed *deltaTime;
		if (direction == UP) {
			this->position -= this->up*velocity;
			this->cameraPos = this->position;
		}
		if (direction == DOWN) {
			this->position += this->up*velocity;
			this->cameraPos = this->position;
		}
		if (direction == LEFT) {
			this->position += this->right*velocity;
			this->cameraPos = this->position;
		}
		if (direction == RIGHT) {
			this->position -= this->right*velocity;
			this->cameraPos = this->position;
		}
		if (direction == SPACE) {
			this->position -= this->up*8.0f*deltaTime;
		}
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
	void Fall(GLfloat deltaTime) {
		if (this->position.y != this->cameraPos.y ){
			this->position += this->up*0.5f*deltaTime;
		}
	}
	glm::vec3 getCameraPos() {
		return this->cameraPos;
	}
	void setPosition(glm::vec3 p) {
		this->position = p;
	}
	glm::vec3 getInitialPos() {
		return this->initialPos;
	}
	void setInitialPos(glm::vec3 p) {
		this->initialPos = p;
	}
private:
	glm::vec3 position;
	glm::vec3 cameraPos;
	glm::vec3 initialPos;
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