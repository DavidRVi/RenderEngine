#include "Model.h"

Model::Model(GLchar* path)
{
	this->loadModel(path);
}

Model::~Model()
{
	meshes.clear();
	textures_loaded.clear();
}

void Model::Draw(Shader* shader)
{
	for (GLuint i = 0; i < meshes.size(); i++)
		this->meshes[i].Draw(shader);
}

void Model::loadModel(string path)
{
	//Create an assimp importer and load the file and save it into an aiScene
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs); 
	// aiProcess_Triangulate forces assimp to transform all primitive shapes to triangles if model is not triangulated. 
	// aiProcess_FlipUVs flip 'y' uv coordinate when necessary due to OpenGL/image format discrepances

	// Check if file is correctly loaded
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR ASSIMP LOADING FILE: " << importer.GetErrorString() << endl;
		return;
	}
	this->directory = path.substr(0, path.find_last_of('/'));

	this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	//Process all node's meshes
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processMesh(mesh, scene));
	}

	//Do the same for its children if any
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> t_vertices;
	vector<GLuint> t_indices;
	vector<Texture*> textures;

	glm::vec3 v_value;
	glm::vec2 textCoord;
	Vertex vertex;
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		//Load vertex position
		v_value.x = mesh->mVertices[i].x;
		v_value.y = mesh->mVertices[i].y;
		v_value.z = mesh->mVertices[i].z;
		vertex.Position = v_value;

		//Load vertex normal if declared
		if (mesh->mNormals)
		{
			v_value.x = mesh->mNormals[i].x;
			v_value.y = mesh->mNormals[i].y;
			v_value.z = mesh->mNormals[i].z;
			vertex.Normal = v_value;
		}
		

		//Load vertex textCoord
		if (mesh->mTextureCoords[0])	//If mesh contains any textCoordinates
		{
			textCoord.x = mesh->mTextureCoords[0][i].x;
			textCoord.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = textCoord;
		}
		else vertex.TexCoords = glm::vec2(0.0f);

		t_vertices.push_back(vertex);
	}

	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
			t_indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex >= 0) // If there's any material defined in the file
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture*> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");

		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		vector<Texture*> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(t_vertices, t_indices, textures);
}

vector<Texture*> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<Texture*> t;
	Texture* text;
	//check amount of textures and iterate.
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		string filename = string(str.C_Str());
		filename = directory + '/' + filename;
		GLboolean skip = false;
		for (GLuint j = 0; j < textures_loaded.size() && !skip; j++)
		{
			if (textures_loaded[j]->getPath() == filename)
			{
				t.push_back(textures_loaded[j]);
				skip = true;
			}
		}
		if (!skip)
		{
			text = new Texture(filename.c_str(), typeName);
			t.push_back(text);
		}
		
	}

	return t;
}

void Model::AddTexture(Texture* t)
{
	textures_loaded.push_back(t);
}

vector<glm::vec3> Model::getVerticesNormals()
{
	vector<glm::vec3> allNormals;
	for (int i = 0; i < meshes.size(); i++)
	{
		vector<glm::vec3> normal = meshes[i].getNormals();
		for (int j = 0; j < normal.size(); j++)
			allNormals.push_back(normal[j]);
	}

	return allNormals;
}

vector<glm::vec3> Model::getVerticesPosition()
{
	vector<glm::vec3> allPositions;
	for (int i = 0; i < meshes.size(); i++)
	{
		vector<glm::vec3> positions = meshes[i].getVertices();
		for (int j = 0; j < positions.size(); j++)
			allPositions.push_back(positions[j]);
	}

	return allPositions;
}

vector<GLuint> Model::getIndices()
{
	vector<GLuint> indices;
	for (int i = 0; i < meshes.size(); i++)
	{
		vector<GLuint> t_indices = meshes[i].getIndices();
		for (int j = 0; j < t_indices.size(); j++)
			indices.push_back(t_indices[j]);
	}

	return indices;
}