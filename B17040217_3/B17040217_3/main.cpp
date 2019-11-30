#include<iostream>

//GLEW
#define GLEW_STATIC

#include<GL/glew.h>

//GLFW
#include<GLFW/glfw3.h>

#include"Shader.h"
#include"Camera.h"
#include"Light.h"

#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/rotate_vector.hpp>

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallBack(GLFWwindow *window, double xPos, double yPos);
void ScrollCallBack(GLFWwindow *window, double xOffset, double yOffset);
void DoMovement();

bool keys[1024];
bool firstMouse = true;

const GLint WIDTH = 800, HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 2.0f));
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
GLfloat lastX = WIDTH / 2;
GLfloat lastY = HEIGHT / 2;
GLfloat aspect = 45.0f;

glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//must for Mac
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);//���ڲ��ɸı��С

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "B17040217", nullptr, nullptr);//�½�����
	if (nullptr == window)//��ʧ�ܣ��չر����ɴ���
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//���ü��̲���
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallBack);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//����ʾ���
	glfwSetScrollCallback(window, ScrollCallBack);//������

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	glfwMakeContextCurrent(window);  //���õ�ǰ����

	glewExperimental = GL_TRUE;
	//��ⴰ���Ƿ��ʼ���ɹ�
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return -1;
	}
	Shader lightShader = Shader("res/shaders/light.vs", "res/shaders/light.frag");
	Light lightModel = Light();

	Shader shader = Shader("res/shaders/core.vs", "res/shaders/core.frag");
	//������Ȳ���
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);//���С��0.5��

	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 0.0f,0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f

	};


	//the data should be transferred to the memory on Graphics Card
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//transfer the data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//set the attribute
	//����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//��ɫ
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glm::mat4 view = glm::mat4(1.0f);

	//glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()),static_cast<GLfloat>(screenWidth)/static_cast<GLfloat>(screenHeight),0.1f,1000.0f);

	while (!glfwWindowShouldClose(window)) {
		//��������رհ�ť����һֱִ��

		GLfloat currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		glfwPollEvents();
		DoMovement();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, screenWidth, screenHeight);
		glm::mat4 projection = glm::perspective(glm::radians(aspect), static_cast<GLfloat>(screenWidth) / static_cast<GLfloat>(screenHeight), 0.1f, 1000.0f);
		view = camera.GetViewMatrix();
		glm::mat4 transform = glm::mat4(1.0f);				//��ʼ����þ���
		lightShader.Use();//ʹ�ù�����ɫ��
		lightPos = glm::vec3(20.0f, 0.0f, 0.0f);
		lightPos = glm::rotate(lightPos, glm::radians(20.0f)*static_cast<GLfloat>(glfwGetTime()), glm::vec3(1.0f, 1.0f, 1.0f));//��ת
		transform = glm::scale(transform, glm::vec3(0.1f, 0.1f, 0.1f));//��������С
		transform = glm::translate(transform, lightPos);

		glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "model"), 1,/*��ת��*/ GL_FALSE, glm::value_ptr(transform));//��transform��ֵ
		glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "view"), 1,/*��ת��*/ GL_FALSE, glm::value_ptr(view));//��transform��ֵ
		glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "projection"), 1,/*��ת��*/ GL_FALSE, glm::value_ptr(projection));//��transform��ֵ

		lightModel.Draw(lightShader);

		shader.Use();
		transform = glm::mat4(1.0f);
		transform = glm::scale(transform, glm::vec3(0.4f, 0.4f, 0.4f));//��������С
		

		GLuint transLoc = glGetUniformLocation(shader.Program, "model");//��ȡtransform������λ��
		glUniformMatrix4fv(transLoc, 1,/*��ת��*/ GL_FALSE, glm::value_ptr(transform));//��transform��ֵ
		GLuint viewLoc = glGetUniformLocation(shader.Program, "view");//��ȡtransform������λ��
		glUniformMatrix4fv(viewLoc, 1,/*��ת��*/ GL_FALSE, glm::value_ptr(view));//��transform��ֵ
		GLuint projLoc = glGetUniformLocation(shader.Program, "projection");//��ȡtransform������λ��
		glUniformMatrix4fv(projLoc, 1,/*��ת��*/ GL_FALSE, glm::value_ptr(projection));//��transform��ֵ

		glUniform3f(glGetUniformLocation(shader.Program, "LightPos"),lightPos.x,lightPos.y,lightPos.z);//��transform��ֵ
		glUniform3f(glGetUniformLocation(shader.Program, "ViewPos"), camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);//��transform��ֵ

		glUniform1f(glGetUniformLocation(shader.Program, "p"), 64.0f);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		//������

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if ((key >= 0) & (key < 1024)) {
		if (action == GLFW_PRESS) {
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			keys[key] = false;
		}
	}
}

void DoMovement() {
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}
void MouseCallBack(GLFWwindow *window, double xPos, double yPos) {
	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = yPos - lastY;


	lastX = xPos;
	lastY = yPos;
	camera.ProcessMouseMovement(xOffset, yOffset);
}

void ScrollCallBack(GLFWwindow * window, double xOffset, double yOffset)
{
	if (aspect >= 1.0f&&aspect <= 45.0f)
		aspect -= yOffset;
	if (aspect <= 1.0f)
		aspect = 1.0f;
	if (aspect >= 45.0f)
		aspect = 45.0f;
}
