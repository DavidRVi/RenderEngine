#ifndef _MESH_H_
#define _MESH_H_

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <glew.h>
#include "Texture.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh
{
private:
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture*> textures;
	
	GLuint VAO, VBO, EBO;

	void setUpMesh();
public:
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture*> textures);
	~Mesh();
	void Draw(Shader* shader);
	vector<glm::vec3> getVertices();
	vector<glm::vec3> getNormals();
	vector<GLuint> getIndices();

};

#endif