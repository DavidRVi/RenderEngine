#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <SOIL.h>
#include <glew.h>
#include <string>

using namespace std;

class Texture{

public:
	Texture(const char* path, string type = "diffuse", GLboolean alpha = false);
	~Texture();
	void Use();
	string getType();
	void setType(string type);
	string getPath();
	GLuint getTextureHandle();
private:

	//Image Res.
	int width, height;
	GLuint m_handle;
	string path;

	string type;
};

#endif