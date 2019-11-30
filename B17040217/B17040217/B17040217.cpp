#include<iostream>

//GLEW
#define GLEW_STATIC
#include<GL/glew.h>

//GLFW
#include<GLFW/glfw3.h>

#include"Shader.h"

const GLint WIDTH = 800, HEIGHT = 600;
/*
const GLchar *vertexShaderSource = "#version 330 core\n"
"layout(location=0) in vec3 position;\n"
"layout(location=1) in vec3 color;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"gl_Position=vec4(position.x,position.y,position.z,1.0f);\n"
"ourColor = color;\n"
"}";

const GLchar *fragmentShaderSource = "#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color=vec4(1.0f,0.5f,0.2f,1.0f);\n"
"color=vec4(ourColor.r,ourColor.g,ourColor.b,1.0f);\n"
"}";
*/
int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//must for Mac
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Learn OpenGL", nullptr,
		nullptr);
	//next two lines are for Mac retina display
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (nullptr == window)//若失败，刚关闭生成窗口
	{
	std::cout << "Failed to create GLFW window" << std :: endl;
	glfwTerminate();
	return -1;
	}
	//检测窗口是否创建成功
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	//检测窗口是否初始化成功
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return -1;
	}
	glViewport(0, 0, screenWidth, screenHeight);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
/*
	//import and compile the shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar inforLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, inforLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPLATION_FAILED\n" << inforLog << std::endl;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, inforLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPLATION_FAILED\n" << inforLog << std::endl;
	}

	//create the program and Link it
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, inforLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << inforLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	*/
	Shader shader = Shader("res/shaders/core.vs", "res/shaders/core.frag");
	//开始画三角形
	//now the vertex information
	/*GLfloat vertices[] = {
		//position                        //color
		-0.5f,-0.5f,0.0f,			1.0f,0.0f,0.0f,
		0.5f,-0.5f,0.0f,			0.0f,1.0f,0.0f,
		0.0f,0.5f,0.0f,				0.0f,0.0f,1.0f
	};*/

	GLfloat vertices[] = {
		/*
		//first triangle
		0.5f,0.5f,0.0f,
		-0.5f,0.5f,0.0f,
		-0.5f,-0.5f,0.0f,
		//second triangle
		0.5f,0.5f,0.0f,
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f
		*/
		0.5f,0.5f,0.0f,
		-0.5f,0.5f,0.0f,
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f
	};

	unsigned int indices[] = {
		0,1,2,
		0,2,3
	};

	//the data should be transfered to the memory on Graphics Card
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//transfer the data
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//set the attribute
	//顶点
	glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),(GLvoid*)0);
	glEnableVertexAttribArray(0);
	//颜色
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);


	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	while (!glfwWindowShouldClose(window)) {
		//若不点击关闭按钮，刚一直执行
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Use();
//		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//重新绑定EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}