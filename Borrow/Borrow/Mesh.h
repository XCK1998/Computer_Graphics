#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

//#include <GL/glew.h>
#include<glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

// 网格至少需要的向量
struct Vertex
{
	// 位置向量
	glm::vec3 Position;
	// 法线向量
	glm::vec3 Normal;
	// 纹理向量
	glm::vec2 TexCoords;
};

// 组织纹理数据
struct Texture
{
	GLuint id;		//纹理id
	string type;	//纹理类型
	aiString path;
};

class Mesh
{
public:
	// Mesh Data
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	// Functions
	// Constructor
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		// Now we have all the data, set the vertex buffers and attribute pointers
		this->setupMesh();
	}

	// 绘制网格
	void Draw(Shader &shader)
	{
		// Bind appropriate textures
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;

		for (GLuint i = 0; i < this->textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);// 在绑定纹理前需要激活适当的纹理单元

			// 检索纹理序列号 (N in diffuse_textureN)
			stringstream ss;
			string number;
			string name = this->textures[i].type;

			if (name == "texture_diffuse") {
				ss << diffuseNr++; // 将GLuin输入到string stream
			}
			if (name == "texture_specular") {
				ss << specularNr++; // 将GLuin输入到string stream
			}

			number = ss.str();

			glUniform1i(glGetUniformLocation(shader.Program, (name + number).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, this->textures[i].id);

		}
		// Assign the shininess
		glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 16.0f);

		// 绘制Mesh
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		for (GLuint i = 0; i < this->textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}


private:
	// Render data
	GLuint VAO, VBO, EBO;

	// 初始化缓冲
	void setupMesh()
	{
		// Create buffers/arrays
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		glBindVertexArray(this->VAO);
		// Load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

		// 将vertex缓冲数据存入
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

		// 设置顶点坐标指针
		// Vertex Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);

		// 设置法线指针
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Normal));

		// 设置顶点的纹理坐标
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);

	}
};
