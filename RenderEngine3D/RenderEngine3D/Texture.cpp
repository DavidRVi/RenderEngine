#include "Texture.h"

Texture::Texture(const char* path, string type, GLboolean alpha)
{

	this->path = string(path);
	//Generate texture
	glGenTextures(1, &m_handle);

	//Bind texture
	glBindTexture(GL_TEXTURE_2D, m_handle);

	// Check if alpha test is selected, if not, load as a regular RGB texture
	unsigned char* image = SOIL_load_image(path, &width, &height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

	//Generate Texture Image using buffered data
	glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//Free image buffer and unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	this->type = type;
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_handle);
}

string Texture::getPath()
{
	return path;
}
void Texture::Use()
{
	glBindTexture(GL_TEXTURE_2D, m_handle);
}

void Texture::setType(string type)
{
	this->type = type;
}

string Texture::getType()
{
	return type;
}

GLuint Texture::getTextureHandle()
{
	return m_handle;
}