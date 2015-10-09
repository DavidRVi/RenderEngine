#ifndef _MODEL_H_
#define _MODEL_H_

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

using namespace std;

class Model
{
private:
	vector<Mesh> meshes;
	string directory;
	vector<Texture*> textures_loaded;

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

public:
	Model(GLchar* path);
	~Model();
	void Draw(Shader* shader);
	void AddTexture(Texture* t);
	vector<glm::vec3> getVerticesPosition();
	vector<glm::vec3> getVerticesNormals();
	vector<GLuint> getIndices();
};
#endif