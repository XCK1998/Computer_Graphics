#pragma once
#ifndef Shader_h
#define Shader_h

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

#include<GL/glew.h>

class Shader
{
	GLuint vertex, fragment;
public:
	GLuint Program;
	//Constructor
	Shader(const GLchar *vertexPath, const GLchar *fragmentPath) {
		//1.Retrieve the source code
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);
		try
		{
			//Open File
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);

			//Stream the File
			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			//Close The File
			vShaderFile.close();
			fShaderFile.close();

			//Convert to the String
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}

		const GLchar *vShaderCode = vertexCode.c_str();
		const GLchar *fShaderCode = fragmentCode.c_str();
		
		//2.Compile the Shaders
		//import and compile the shaders
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		GLint success;
		GLchar inforLog[512];
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, inforLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPLATION_FAILED\n" << inforLog << std::endl;
		}

		fragment= glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, inforLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPLATION_FAILED\n" << inforLog << std::endl;
		}

		//create the program and Link it
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		glLinkProgram(this->Program);
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(this->Program, 512, NULL, inforLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << inforLog << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	//Deconstructor
	~Shader() {
		glDetachShader(this->Program, vertex);
		glDetachShader(this->Program, fragment);
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		glDeleteProgram(this->Program);
	}
	void Use() {
		glUseProgram(this->Program);
	}	
private:

};

#endif // !Shader_h
