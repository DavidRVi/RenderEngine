#include "camera.h"

Camera::Camera(GLFWwindow** w)
{

	window = w;
	position = glm::vec3(0, 0, 5);
	horizontalAngle = 3.14f;
	verticalAngle = 0.0f;
	initialFoV = 45.0f;

	speed = 3.0f; // 3 units / second
	mouseSpeed = 0.005f;

	// Calculamos el vector dirección, ¿hacia dónde mira la cámara?
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

	// Calculamos el vector derecha para ayudarnos a conocer el vector up de la cámara para la orientación
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
		);

	// Producto vectorial del vector dirección y derecha nos da el vector perpendicular (vector up) a estos
	glm::vec3 up = glm::cross(right, direction);


	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projectionMatrix = glm::perspective(initialFoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	viewMatrix = glm::lookAt(
		position,
		position + direction,
		up
		);
}

Camera::~Camera()
{
}

glm::mat4 Camera::getProjectionMatrix()
{
	return projectionMatrix;
}

glm::mat4 Camera::getViewMatrix()
{
	return viewMatrix;
}

glm::vec3 Camera::getPosition()
{
	return position;
}

void Camera::ComputeMatricesFromInputs()
{
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position and window's width & height
	double xpos, ypos;
	int width, height;
	glfwGetWindowSize(*window, &width, &height);
	glfwGetCursorPos(*window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(*window, width / 2, height / 2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(width / 2 - xpos);
	verticalAngle += mouseSpeed * float(height / 2 - ypos);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
		);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	// Move forward
	if (glfwGetKey(*window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(*window, GLFW_KEY_W) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey(*window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(*window, GLFW_KEY_S) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(*window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(*window, GLFW_KEY_D) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(*window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(*window, GLFW_KEY_A) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projectionMatrix = glm::perspective(initialFoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	viewMatrix = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
		);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;

}
