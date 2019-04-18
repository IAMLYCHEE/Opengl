#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include<glm.hpp>
class Shader
{
public:
	unsigned int ID;
	Shader() {}
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	Shader(const std::string& vertexPath,
		const std::string& tcsPath,
		const std::string& tesPath,
		const std::string& gsPath,
		const std::string& fragmentPath)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string tcsCode;
		std::string tesCode;
		std::string gsCode;
		std::string fragmentCode;

		std::ifstream vShaderFile;
		std::ifstream tcsShaderFile;
		std::ifstream tesShaderFile;
		std::ifstream gsShaderFile;
		std::ifstream fShaderFile;

		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		tcsShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		tesShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gsShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			// open files
			vShaderFile.open(vertexPath);
			tcsShaderFile.open(tcsPath);
			tesShaderFile.open(tesPath);
			gsShaderFile.open(gsPath);
			fShaderFile.open(fragmentPath);

			std::stringstream vShaderStream;
			std::stringstream tcsShaderStream;
			std::stringstream tesShaderStream;
			std::stringstream gsShaderStream;
			std::stringstream fShaderStream;

			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			tcsShaderStream << tcsShaderFile.rdbuf();
			tesShaderStream << tesShaderFile.rdbuf();
			gsShaderStream << gsShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			// close file handlers
			vShaderFile.close();
			tcsShaderFile.close();
			tesShaderFile.close();
			gsShaderFile.close();
			fShaderFile.close();

			// convert stream into string
			vertexCode = vShaderStream.str();
			tcsCode = tcsShaderStream.str();
			tesCode = tesShaderStream.str();
			gsCode = gsShaderStream.str();
			fragmentCode = fShaderStream.str();

		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}



		// 2. compile shaders
		GLuint vertexShader;
		GLuint tcsShader;
		GLuint tesShader;
		GLuint gsShader;
		GLuint fragmentShader;

		bool vertexShaderActivated = false;
		bool tcsShaderActivated = false;
		bool tesShaderActivated = false;
		bool gsShaderActivated = false;
		bool fragmentShaderActivated = false;

		if (!vertexCode.empty())
		{
			const char* vShaderCode = vertexCode.c_str();
			vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vShaderCode, NULL);
			glCompileShader(vertexShader);
			checkCompileErrors(vertexShader, "VERTEX");
			vertexShaderActivated = true;
		}//vertex shader

		if (!tcsCode.empty())
		{
			const char* tcsShaderCode = tcsCode.c_str();
			tcsShader = glCreateShader(GL_TESS_CONTROL_SHADER);
			glShaderSource(tcsShader, 1, &tcsShaderCode, NULL);
			glCompileShader(tcsShader);
			checkCompileErrors(tcsShader, "TESS_CONTROL");
			tcsShaderActivated = true;
		}//tessellation control shader

		if (!tesCode.empty())
		{
			const char* tesShaderCode = tesCode.c_str();
			tesShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
			glShaderSource(tesShader, 1, &tesShaderCode, NULL);
			glCompileShader(tesShader);
			checkCompileErrors(tesShader, "TESS_EVALUATION");
			tesShaderActivated = true;
		}//tessellation evaluation shader

		if (!gsCode.empty())
		{
			const char* gsShaderCode = gsCode.c_str();
			gsShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(gsShader, 1, &gsShaderCode, NULL);
			glCompileShader(gsShader);
			checkCompileErrors(gsShader, "GEOMETRY");
			gsShaderActivated = true;
		}//geometry shader

		if (!fragmentCode.empty())
		{
			const char* fsShaderCode = fragmentCode.c_str();
			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fsShaderCode, NULL);
			glCompileShader(fragmentShader);
			checkCompileErrors(fragmentShader, "FRAGMENT");
			fragmentShaderActivated = true;
		}//tessellation control shader

		// shader Program
		ID = glCreateProgram();
		if (vertexShaderActivated) {
			glAttachShader(ID, vertexShader);
		}
		if (tcsShaderActivated) {
			glAttachShader(ID, tcsShader);
		}
		if (tesShaderActivated) {
			glAttachShader(ID, tesShader);
		}
		if (gsShaderActivated) {
			glAttachShader(ID, gsShader);
		}
		if (fragmentShaderActivated) {
			glAttachShader(ID, fragmentShader);
		}

		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessary
		if (vertexShaderActivated) {
			glDeleteShader(vertexShader);
			vertexShaderActivated = false;
		}
		if (tcsShaderActivated) {
			glDeleteShader(tcsShader);
			tcsShaderActivated = false;
		}
		if (tesShaderActivated) {
			glDeleteShader(tesShader);
			tesShaderActivated = false;
		}
		if (gsShaderActivated) {
			glDeleteShader(gsShader);
			gsShaderActivated = false;
		}
		if (fragmentShaderActivated) {
			glDeleteShader(fragmentShader);
			fragmentShaderActivated = false;
		}
	}
	// activate the shader
	// ------------------------------------------------------------------------
	void use()
	{
		glUseProgram(ID);
	}
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setUniformMat4f(const std::string& name, glm::mat4& mat) {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}


private:
	// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};
#endif
