#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#pragma once 

#include <glew.h>
#include <SOIL.h>
#include <string>
#include <vector>
#include "Shader.h"

using namespace std;

class Skybox {
private:
	GLuint cubemap_id;	//Handle for the cubemap texture
	GLuint VAO, VBO;

public:
	// Constructor with cubemap path
	Skybox(string path);
	void Draw(Shader* shader);
	GLuint getTexture();
	~Skybox();

};

#endif