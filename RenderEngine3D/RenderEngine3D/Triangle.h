#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "Shader.h"
#include <glew.h>

class Triangle {
public:
	Triangle();
	void Draw(Shader *shader);
private:
	GLuint VAO, VBO;
};

#endif