#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

class Light
{

private:
	glm::vec3 position;
	glm::vec3 color;
	float ambientS;
	float diffuseS;
	float specularS;

public:
	Light(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 col = glm::vec3(1.0f), float amb = 0.2f, float diff = 0.5f, float spec = 0.2f);
	~Light();
	glm::vec3 getPos();
	glm::vec3 getColor();
	float getAmbientS();
	float getDiffuseS();
	float getSpecularS();
	void setPosition(glm::vec3 newPos);
	glm::vec3 movePos(glm::vec3 translation);
	void setColor(glm::vec3 newColor);
	void setAmbientS(float newA);
	void setDiffuseS(float newD);
	void setSpecularS(float newS);
	void rotateLight(float degrees);

};
#endif