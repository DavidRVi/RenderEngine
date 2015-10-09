#include "Shader.h"

using namespace std;
Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFile;

	vShaderFile.exceptions(ifstream::badbit);
	fShaderFile.exceptions(ifstream::badbit);

	try
	{

		//Open both shaders
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		//Create stream to read both shaders
		stringstream vShaderStream, fShaderStream;

		//Read file into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (ifstream::failure e)
	{
		cout << "ERROR READING SHADER FILE: " << e.what() << endl;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	//Compile Shaders

	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	//Get Compile status and see if its successful
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		cout << "ERROR COMPILING VERTEX SHADER:\n" << infoLog << endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	//Get Compile status and see if its successful
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		cout << "ERROR COMPILING FRAGMENT SHADER:\n" << infoLog << endl;
	}

	//Create shader program and link compiled shaders
	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertex);
	glAttachShader(this->Program, fragment);
	glLinkProgram(this->Program);

	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		cout << "ERROR LINKING SHADER PROGRAM: \n" << infoLog << endl;
	}
	//Once Linked, there's no need to keep the shaders in memory
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
	glDeleteProgram(this->Program);
}

void Shader::Use()
{
	glUseProgram(this->Program);
}

void Shader::assignLoc1f(char* loc, float v)
{
	glUniform1f(glGetUniformLocation(this->Program, loc), v);
}

void Shader::assignLoc3f(char* loc, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(this->Program, loc), x, y, z);
}