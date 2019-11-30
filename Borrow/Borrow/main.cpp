#include <iostream>
//GLEW
//#include <GL/glew.h>
//#define GLEW_STATIC
#include<glad/glad.h>
//GLFW
#include <GLFW/glfw3.h>

//SOIL
#include "SOIL2/SOIL2.h"
#include "SOIL2/stb_image.h"
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Mesh.h"
#include "Light.h"
#include "skybox.h"

const GLint pickSize = 32;

const GLint WIDTH = 1200, HEIGHT = 1200;

//��Ӧ���̲���
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
//��Ӧ���ֲ���
void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset);
//��Ӧ������
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
//ִ�в���
void DoMovement();
//�ظ�����
void Repeat();
//�ط�
void Reback();
int transDirection(int direction);


//��ʼ��һ�������
Camera camera(glm::vec3(0.0f, 0.0f, 2.0f));
//��ʼ�ӽ�����Ļ����
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;


bool keys[1024];		//��ȡ�洢�ļ��̲���
int keyStores[102400];	//����ÿһ�εİ���
int index = 0;
int temp = 0;
bool repeat = false;	//�ظ���һ�ζ���
bool reback = false;	//�ص�֮ǰ��λ��


GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
bool firstMouse = true;



int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "B16040227", nullptr, nullptr);

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (nullptr == window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);			//�����ڵ������Ļ�������Ϊ��ǰ���̵߳������Ļ���

	// Set the required callback function
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);		//�������
	/*glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return -1;
	}*/
	glViewport(0, 0, screenWidth, screenHeight);;  //�ӣ�0,0����ʼ���㣬ֱ�� WIDTH �� HEIGHT

	glEnable(GL_BLEND);			//����͸���Ȼ�ϣ��̶����ܸ�
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		//alpha ���Ի�ϣ����õ�ǰΪ �� ��������Ϊ 1- ��
	glEnable(GL_DEPTH_TEST);		//��Ȳ���
//	glDepthFunc(GL_LESS);			//�����ϢС�ڵ�ǰ��Ϣ���ͽ��в���
	glDepthFunc(GL_LEQUAL);			//�����ϢС�ڵ��ڵ�ǰ��Ϣ���ͽ��в���

	Shader shader("shaders/modelLoad.vs", "shaders/modelLoad.frag");
	Shader lightShader("shaders/core.vs", "shaders/core.frag");
	Shader skyboxShader("shaders/skybox.vs", "shaders/skybox.frag");


	// Load models
	Model ourModel("models/nanosuit.obj");
	Light lightModel = Light();

	Skybox skybox = Skybox();

	glm::vec3 LightPos = glm::vec3(1.5f, 1.5f, 0.0f);		//��Դλ��


	// ����Skybox
	vector<const GLchar*> faces;
	faces.push_back("skybox/right.jpg");
	faces.push_back("skybox/left.jpg");
	faces.push_back("skybox/top.jpg");
	faces.push_back("skybox/bottom.jpg");
	faces.push_back("skybox/back.jpg");
	faces.push_back("skybox/front.jpg");
	GLuint cubemapTexture = skybox.loadSkyBox(faces);


	while (!glfwWindowShouldClose(window)) {

		// Get the delta time between frames
		GLfloat currentFrame = glfwGetTime();		//��Ļ�ջ�������ʱ��
		deltaTime = currentFrame - lastTime;
		lastTime = currentFrame;

		LightPos = glm::rotate(LightPos, 0.005f, glm::vec3(0.0f, 1.0f, 0.0f));		//��Դ���ٶȣ���ת��xyz
		glfwPollEvents();			//��ȡ�����̲���
		DoMovement();				//��Ӧ�����̲���

		Repeat();
		Reback();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//������ɫ�����

		// ���Ȼ�����պ���
		glDepthMask(GL_FALSE);// �ر����
		skyboxShader.Use();
		glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(skybox.VAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);

		view = camera.GetViewMatrix();
		//ĳЩGLMΪ��������Ҫ��ת��
		projection = glm::perspective(glm::radians(camera.GetZoom()), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 model = glm::mat4(1.0f);
		lightShader.Use();
		model = glm::translate(model, LightPos);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

		glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		lightModel.Draw(lightShader);		//���ƹ�Դ
			
		shader.Use();
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(glGetUniformLocation(shader.Program, "ViewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(shader.Program, "LightPos"), LightPos.x, LightPos.y, LightPos.z);
		glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 64.0f);
		ourModel.Draw(shader);
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS) {
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

}

void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll(yOffset);
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}
	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = yPos - lastY;

	lastX = xPos;
	lastY = yPos;

	// Process the camera direction
	camera.ProcessMouseMovement(xOffset, yOffset);
}

void DoMovement()
{
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
		if (!repeat && !reback) {
			keyStores[index] = GLFW_KEY_UP;
			index++;
		}
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
		if (!repeat && !reback) {
			keyStores[index] = GLFW_KEY_DOWN;
			index++;
		}
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
		if (!repeat && !reback) {
			keyStores[index] = GLFW_KEY_LEFT;
			index++;
		}
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
		if (!repeat && !reback) {
			keyStores[index] = GLFW_KEY_RIGHT;
			index++;
		}
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	if (keys[GLFW_KEY_R]) {
		repeat = true;
	}

	if (keys[GLFW_KEY_B]) {
		reback = true;
		temp = index - 1;
	}
}

void Repeat()
{
	if (repeat && keyStores[index - 1] != 0) {
		if (temp < index - 1) {
			if (temp == 0) {
				keys[keyStores[temp]] = true;
				temp++;
			}
			else {
				keys[keyStores[temp - 1]] = false;
				keyStores[temp - 1] = 0;
				keys[keyStores[temp]] = true;
				temp++;

			}
		}
		else {
			keys[keyStores[temp - 1]] = false;
			keyStores[temp - 1] = 0;
			repeat = false;
			index = 0;
			temp = 0;
		}
	}
}

void Reback()
{
	if (reback && keyStores[0] != 0) {
		if (temp > -1) {
			if (temp == index - 1) {
				int tran = transDirection(keyStores[temp]);
				keys[tran] = true;
				temp--;
			}
			else {
				int tran = transDirection(keyStores[temp + 1]);
				keys[tran] = false;
				keyStores[temp + 1] = 0;
				tran = transDirection(keyStores[temp]);
				keys[tran] = true;
				temp--;
			}
		}
		else {
			int tran = transDirection(keyStores[temp + 1]);
			keys[tran] = false;
			keyStores[temp + 1] = 0;
			reback = false;
			index = 0;
		}
	}
}

int transDirection(int direction)
{
	if (direction == GLFW_KEY_UP) {
		return GLFW_KEY_DOWN;
	}
	if (direction == GLFW_KEY_DOWN) {
		return GLFW_KEY_UP;
	}
	if (direction == GLFW_KEY_RIGHT) {
		return GLFW_KEY_LEFT;
	}
	if (direction == GLFW_KEY_LEFT) {
		return GLFW_KEY_RIGHT;
	}
}

