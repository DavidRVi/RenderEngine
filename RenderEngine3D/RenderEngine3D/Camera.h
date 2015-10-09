#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

// Clase que controlará la cámara en nuestra aplicación
class Camera{
private:

	//Referencia a la ventana activa
	GLFWwindow** window;

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	// Initial position : on +Z
	glm::vec3 position;
	// Initial horizontal angle : toward -Z
	float horizontalAngle;
	// Initial vertical angle : none
	float verticalAngle;
	// Initial Field of View
	float initialFoV;

	float speed;
	float mouseSpeed;

public:
	Camera(GLFWwindow** w);
	~Camera();

	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();
	void ComputeMatricesFromInputs();
	glm::vec3 getPosition();

};

#endif