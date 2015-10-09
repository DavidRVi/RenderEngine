#include "Light.h"

Light::Light(glm::vec3 pos, glm::vec3 col, float amb, float diff, float spec)
{
	position = pos;
	color = col;
	ambientS = amb;
	diffuseS = diff;
	specularS = spec;
}

Light::~Light()
{

}

void Light::setPosition(glm::vec3 newPos)
{
	position = newPos;
}

void Light::setColor(glm::vec3 newColor)
{
	color = newColor;
}

	
glm::vec3 Light::getPos()
{
	return position;
}

glm::vec3 Light::getColor()
{
	return color;
}

float Light::getAmbientS()
{
	return ambientS;
}

float Light::getDiffuseS()
{
	return diffuseS;
}

float Light::getSpecularS()
{
	return specularS;
}


glm::vec3 Light::movePos(glm::vec3 translation)
{
	position += translation;
	return position;
}

void Light::setAmbientS(float newA)
{
	ambientS = newA;
}

void Light::setDiffuseS(float newD)
{
	diffuseS = newD;
}

void Light::setSpecularS(float newS)
{
	specularS = newS;
}

void Light::rotateLight(float degrees)
{
	glm::vec4 rotationLight = glm::rotate(glm::mat4(1.0f), degrees, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(position, 1.0f);
	position = glm::vec3(rotationLight.x, rotationLight.y, rotationLight.z);
}