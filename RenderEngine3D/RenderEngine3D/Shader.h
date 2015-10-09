#ifndef _SHADER_H_
#define _SHADER_H_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL\glew.h>

class Shader
{
public:

	//The shader program
	GLuint Program;
	//Constructor with file path of both shaders
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	~Shader();

	//Use this shader program
	void Use();
	//assign float uniform variable with name "locname"
	void assignLoc1f(char* locname, float v);
	//assign vec3 uniform variable with name "locname"
	void assignLoc3f(char* locname, float x, float y, float z);
};

#endif