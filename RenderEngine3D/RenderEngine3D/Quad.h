#ifndef _QUAD_H_
#define _QUAD_H_


#include <glew.h>
#include "Shader.h"
#include "Texture.h"


class Quad{
private:
	GLuint VAO, VBO;
	Texture *t;
public:
	Quad(char* path);
	~Quad();
	GLuint getTexture();
	void Draw(Shader* shader);
};

#endif