#include "Mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture*> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setUpMesh();
}

Mesh::~Mesh()
{
	vertices.clear();
	indices.clear();
	textures.clear();
}

void Mesh::setUpMesh()
{
	// Generate every array
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	//Bind this VAO and start filling the VBO
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

	//fill the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

void Mesh::Draw(Shader* shader)
{
	shader->Use();
	// Try to get the number of diffuse and specular textures the mesh is going to use
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		// Active Texture Unit 0+i
		glActiveTexture(GL_TEXTURE0 + i);

		// Get texture name and number
		stringstream ss;
		string number;
		string name = this->textures[i]->getType();

		// Texture types are: diffuse || specular
		if (name == "diffuse")
			ss << diffuseNr++;
		else if (name == "specular")
			ss << specularNr++;
		number = ss.str();

		// Link texture with "nameNumber" sampler2D on shader
		glUniform1i(glGetUniformLocation(shader->Program, (name + number).c_str()), i);
		this->textures[i]->Use();
	}
	glActiveTexture(GL_TEXTURE0);

	//Draw the elements
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Reset Texture units status
	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

vector<glm::vec3> Mesh::getVertices()
{
	vector<glm::vec3> verticesPos;
	for (int i = 0; i < vertices.size(); i++)
		verticesPos.push_back(vertices[i].Position);

	return verticesPos;
}

vector<glm::vec3> Mesh::getNormals()
{
	vector<glm::vec3> normals;
	for (int i = 0; i < vertices.size(); i++)
		normals.push_back(vertices[i].Normal);

	return normals;
}

vector<GLuint> Mesh::getIndices()
{
	return indices;
}