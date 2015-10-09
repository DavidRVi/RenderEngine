// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>


// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Light.h"
#include "Model.h"
#include "Skybox.h"
#include "Quad.h"

#include "Triangle.h"

int main(void)
{
	GLFWwindow* window;
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "3D Engine", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Dark blue background
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it's closer to the camera than the former one (depth test <)
	glDepthFunc(GL_LESS);

	Model	*lightModel 	= new Model("models/cube.obj");
	Model	*monkeyModel	= new Model("models/monkey.obj");
	Model	*teapot			= new Model("models/teapot.obj");
	Model	*sphere			= new Model("models/sphere.obj");

	// Create and compile our GLSL programs from the shaders
	Shader *wireframe				= new Shader("shaders/wireframe.vs", "shaders/wireframe.fs");
	Shader *solid					= new Shader("shaders/solid.vs", "shaders/solid.fs");
	Shader *lighting				= new Shader("shaders/lighting.vs", "shaders/lighting.fs");
	Shader *gouraud					= new Shader("shaders/gouraud.vs", "shaders/gouraud.fs");
	Shader *phong					= new Shader("shaders/phong.vs", "shaders/phong.fs");
	Shader *textured				= new Shader("shaders/phong_texture.vs", "shaders/phong_texture.fs");
	Shader *normal_textured			= new Shader("shaders/phong_normal.vs", "shaders/phong_normal.fs");
	Shader *skybox_shader			= new Shader("shaders/skybox.vs", "shaders/skybox.fs");
	Shader *reflection				= new Shader("shaders/reflection.vs", "shaders/reflection.fs");
	Shader *refraction				= new Shader("shaders/reflection.vs", "shaders/refraction.fs");
	Shader *phong_transparency		= new Shader("shaders/phong_transparency.vs", "shaders/phong_transparency.fs");
	Shader *transparency			= new Shader("shaders/transparency.vs", "shaders/transparency.fs");

	Texture *normalMap		= new Texture("textures/brick_normal.jpg");
	Texture *text			= new Texture("textures/brick_diffuse.jpg");
	Texture *sphere_mask	= new Texture("textures/mask.png", "diffuse", true);
	Texture *mask			= new Texture("textures/window.png", "diffuse", true);

	Skybox  *skybox = new Skybox("textures/skybox/");
	Quad	*wall	= new Quad("textures/window.png");

	Triangle	*triangle = new Triangle();
	// Get a handle for our "MVP" uniform in the lighting shader program
	GLuint MVP_lighting = glGetUniformLocation(lighting->Program, "MVP");
	
	float degrees = 0.0f;
	double timeWall = 1;
	double lastTime = 0;
	Camera *camera = new Camera(&window);
	
	Light *light = new Light(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = light->getPos();
	glm::vec3 lightColor = light->getColor();

	enum renderMode{ WIREFRAME, SOLID, GOURAUD, PHONG, TEXTURE, NORMAL_TEXTURE, SKYBOX, REFLECTION, REFRACTION, ALPHA_BLENDING };
	renderMode currentRenderMode = SOLID;
	renderMode lastRenderMode = currentRenderMode;

	do{
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			light->rotateLight(-0.005f);
			lightPos = light->getPos();
		}
		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		{
			degrees += 0.0010;
		}
		// Clear the screen
		glClearColor(.0f, .0f, .4f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera->ComputeMatricesFromInputs();
		glm::vec3 cameraPos = camera->getPosition();
		glm::mat4 ProjectionMatrix = camera->getProjectionMatrix();
		glm::mat4 ViewMatrix = camera->getViewMatrix();
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0), degrees, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 ModelMatrix = rotationMatrix * scaleMatrix * glm::mat4(1.0);

		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		switch (currentRenderMode)
		{
		case WIREFRAME:
			wireframe->Use();
			glUniformMatrix4fv(glGetUniformLocation(wireframe->Program, "MVP"), 1, GL_FALSE, &MVP[0][0]);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			monkeyModel->Draw(wireframe);
			break;
		case SOLID:
			solid->Use();
			glUniformMatrix4fv(glGetUniformLocation(solid->Program, "MVP"), 1, GL_FALSE, &MVP[0][0]);
			//glUniformMatrix4fv(glGetUniformLocation(solid->Program, "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			monkeyModel->Draw(solid);
			break;
		case GOURAUD:
			gouraud->Use();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glUniformMatrix4fv(glGetUniformLocation(gouraud->Program, "MVP"), 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(gouraud->Program, "M"), 1, GL_FALSE, &ModelMatrix[0][0]);
			gouraud->assignLoc3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
			gouraud->assignLoc3f("light.color", lightColor.x, lightColor.y, lightColor.z);
			gouraud->assignLoc3f("light.position", lightPos.x, lightPos.y, lightPos.z);
			gouraud->assignLoc1f("light.ambient", light->getAmbientS());
			gouraud->assignLoc1f("light.diffuse", light->getDiffuseS());
			gouraud->assignLoc1f("light.specular", light->getSpecularS());
			gouraud->assignLoc3f("material.ambient", 0.25f, 0.25f, 0.25f);
			gouraud->assignLoc3f("material.diffuse", 0.4f, 0.4f, 0.4f);
			gouraud->assignLoc3f("material.specular", 0.774597f, 0.774597f, 0.774597f);
			gouraud->assignLoc1f("material.shininess", 0.6*128.0f);
			monkeyModel->Draw(gouraud);
			lighting->Use();
			ModelMatrix = glm::translate(glm::mat4(1.0f), lightPos) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			lighting->assignLoc3f("lightColor", lightColor.x, lightColor.y, lightColor.z);
			glUniformMatrix4fv(MVP_lighting, 1, GL_FALSE, &MVP[0][0]);
			lightModel->Draw(lighting);
			break;
		case PHONG:
			phong->Use();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glUniformMatrix4fv(glGetUniformLocation(phong->Program, "MVP"), 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(phong->Program, "M"), 1, GL_FALSE, &ModelMatrix[0][0]);
			phong->assignLoc3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
			phong->assignLoc3f("light.color", lightColor.x, lightColor.y, lightColor.z);
			phong->assignLoc3f("light.position", lightPos.x, lightPos.y, lightPos.z);
			phong->assignLoc1f("light.ambient", light->getAmbientS());
			phong->assignLoc1f("light.diffuse", light->getDiffuseS());
			phong->assignLoc1f("light.specular", light->getSpecularS());
			phong->assignLoc3f("material.ambient", 0.25f, 0.25f, 0.25f);
			phong->assignLoc3f("material.diffuse", 0.4f, 0.4f, 0.4f);
			phong->assignLoc3f("material.specular", 0.774597f, 0.774597f, 0.774597f);
			phong->assignLoc1f("material.shininess", 0.6*128.0f);
			monkeyModel->Draw(phong);
			lighting->Use();
			ModelMatrix = glm::translate(glm::mat4(1.0f), lightPos) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			lighting->assignLoc3f("lightColor", lightColor.x, lightColor.y, lightColor.z);
			glUniformMatrix4fv(MVP_lighting, 1, GL_FALSE, &MVP[0][0]);
			lightModel->Draw(lighting);
			break;
		case TEXTURE:
			textured->Use();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glUniformMatrix4fv(glGetUniformLocation(textured->Program, "MVP"), 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(textured->Program, "M"), 1, GL_FALSE, &ModelMatrix[0][0]);
			textured->assignLoc3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
			textured->assignLoc3f("light.color", lightColor.x, lightColor.y, lightColor.z);
			textured->assignLoc3f("light.position", lightPos.x, lightPos.y, lightPos.z);
			textured->assignLoc1f("light.ambient", light->getAmbientS());
			textured->assignLoc1f("light.diffuse", light->getDiffuseS());
			textured->assignLoc1f("light.specular", light->getSpecularS());
			glActiveTexture(GL_TEXTURE0);
			text->Use();
			glUniform1i(glGetUniformLocation(textured->Program, "diffTexture"), 0);
			sphere->Draw(textured);
			lighting->Use();
			ModelMatrix = glm::translate(glm::mat4(1.0f), lightPos) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			lighting->assignLoc3f("lightColor", lightColor.x, lightColor.y, lightColor.z);
			glUniformMatrix4fv(MVP_lighting, 1, GL_FALSE, &MVP[0][0]);
			lightModel->Draw(lighting);
			break;
		case NORMAL_TEXTURE:
			normal_textured->Use();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glUniformMatrix4fv(glGetUniformLocation(normal_textured->Program, "MVP"), 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(normal_textured->Program, "M"), 1, GL_FALSE, &ModelMatrix[0][0]);
			normal_textured->assignLoc3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
			normal_textured->assignLoc3f("light.color", lightColor.x, lightColor.y, lightColor.z);
			normal_textured->assignLoc3f("light.position", lightPos.x, lightPos.y, lightPos.z);
			normal_textured->assignLoc1f("light.ambient", light->getAmbientS());
			normal_textured->assignLoc1f("light.diffuse", light->getDiffuseS());
			normal_textured->assignLoc1f("light.specular", light->getSpecularS());
			glActiveTexture(GL_TEXTURE0);
			text->Use();
			glUniform1i(glGetUniformLocation(normal_textured->Program, "diffTexture"), 0);
			glActiveTexture(GL_TEXTURE1);
			normalMap->Use();
			glUniform1i(glGetUniformLocation(normal_textured->Program, "normalTexture"), 1);
			sphere->Draw(normal_textured);
			lighting->Use();
			ModelMatrix = glm::translate(glm::mat4(1.0f), lightPos) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			lighting->assignLoc3f("lightColor", lightColor.x, lightColor.y, lightColor.z);
			glUniformMatrix4fv(MVP_lighting, 1, GL_FALSE, &MVP[0][0]);
			lightModel->Draw(lighting);
			break;
		case SKYBOX:
			gouraud->Use();
			gouraud->assignLoc3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
			gouraud->assignLoc3f("light.color", lightColor.x, lightColor.y, lightColor.z);
			gouraud->assignLoc3f("light.position", lightPos.x, lightPos.y, lightPos.z);
			gouraud->assignLoc1f("light.ambient", light->getAmbientS());
			gouraud->assignLoc1f("light.diffuse", light->getDiffuseS());
			gouraud->assignLoc1f("light.specular", light->getSpecularS());
			gouraud->assignLoc3f("material.ambient", 0.5f, 0.5f, 0.0f);
			gouraud->assignLoc3f("material.diffuse", 0.7f, 0.7f, 0.0f);
			gouraud->assignLoc3f("material.specular", 0.6f, 0.6f, 0.5f);
			gouraud->assignLoc1f("material.shininess", 0.25*128.0f);
			glUniformMatrix4fv(glGetUniformLocation(gouraud->Program, "M"), 1, GL_FALSE, &ModelMatrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(gouraud->Program, "MVP"), 1, GL_FALSE, &MVP[0][0]);
			teapot->Draw(gouraud);
			glDepthFunc(GL_LEQUAL);
			skybox_shader->Use();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			ViewMatrix = glm::mat4(glm::mat3(ViewMatrix));
			glUniformMatrix4fv(glGetUniformLocation(skybox_shader->Program, "view"), 1, GL_FALSE, &ViewMatrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(skybox_shader->Program, "projection"), 1, GL_FALSE, &ProjectionMatrix[0][0]);
			skybox->Draw(skybox_shader);
			glDepthFunc(GL_LESS);
			
			break;
		case REFLECTION:
			glDepthFunc(GL_LEQUAL);
			reflection->Use();
			glUniformMatrix4fv(glGetUniformLocation(reflection->Program, "MVP"), 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(reflection->Program, "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
			reflection->assignLoc3f("cameraPos", cameraPos.x, cameraPos.y, cameraPos.z);
			glActiveTexture(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTexture());
			teapot->Draw(reflection);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			skybox_shader->Use();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			ViewMatrix = glm::mat4(glm::mat3(ViewMatrix));
			glUniformMatrix4fv(glGetUniformLocation(skybox_shader->Program, "view"), 1, GL_FALSE, &ViewMatrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(skybox_shader->Program, "projection"), 1, GL_FALSE, &ProjectionMatrix[0][0]);
			skybox->Draw(skybox_shader);
			glDepthFunc(GL_LESS);
			break;
		case REFRACTION:
			glDepthFunc(GL_LEQUAL);
			refraction->Use();
			glUniformMatrix4fv(glGetUniformLocation(refraction->Program, "MVP"), 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(refraction->Program, "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
			refraction->assignLoc3f("cameraPos", cameraPos.x, cameraPos.y, cameraPos.z);
			glActiveTexture(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTexture());
			sphere->Draw(refraction);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			skybox_shader->Use();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			ViewMatrix = glm::mat4(glm::mat3(ViewMatrix));
			glUniformMatrix4fv(glGetUniformLocation(skybox_shader->Program, "view"), 1, GL_FALSE, &ViewMatrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(skybox_shader->Program, "projection"), 1, GL_FALSE, &ProjectionMatrix[0][0]);
			skybox->Draw(skybox_shader);
			glDepthFunc(GL_LESS);
			break;
		case ALPHA_BLENDING:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			//ALPHA_BLENDING
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			transparency->Use();
			glUniformMatrix4fv(glGetUniformLocation(transparency->Program, "view"), 1, GL_FALSE, &ViewMatrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(transparency->Program, "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(transparency->Program, "projection"), 1, GL_FALSE, &ProjectionMatrix[0][0]);

			lighting->Use();
			ModelMatrix = glm::translate(glm::mat4(1.0f), lightPos) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			lighting->assignLoc3f("lightColor", lightColor.x, lightColor.y, lightColor.z);
			glUniformMatrix4fv(MVP_lighting, 1, GL_FALSE, &MVP[0][0]);
			lightModel->Draw(lighting);

			// Draw order is very important so we draw first what's further away from the camera
			
			gouraud->Use();
			gouraud->assignLoc3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
			gouraud->assignLoc3f("light.color", lightColor.x, lightColor.y, lightColor.z);
			gouraud->assignLoc3f("light.position", lightPos.x, lightPos.y, lightPos.z);
			gouraud->assignLoc1f("light.ambient", light->getAmbientS());
			gouraud->assignLoc1f("light.diffuse", light->getDiffuseS());
			gouraud->assignLoc1f("light.specular", light->getSpecularS());
			gouraud->assignLoc3f("material.ambient", 0.5f, 0.5f, 0.0f);
			gouraud->assignLoc3f("material.diffuse", 0.7f, 0.7f, 0.0f);
			gouraud->assignLoc3f("material.specular", 0.6f, 0.6f, 0.5f);
			gouraud->assignLoc1f("material.shininess", 0.25*128.0f);
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -1.0f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(gouraud->Program, "M"), 1, GL_FALSE, &ModelMatrix[0][0]);
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			glUniformMatrix4fv(glGetUniformLocation(gouraud->Program, "MVP"), 1, GL_FALSE, &MVP[0][0]);
			teapot->Draw(gouraud);

			phong_transparency->Use();
			glUniformMatrix4fv(glGetUniformLocation(phong_transparency->Program, "view"), 1, GL_FALSE, &ViewMatrix[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(phong_transparency->Program, "projection"), 1, GL_FALSE, &ProjectionMatrix[0][0]);
			phong_transparency->assignLoc3f("light.color", lightColor.x, lightColor.y, lightColor.z);
			phong_transparency->assignLoc3f("light.position", lightPos.x, lightPos.y, lightPos.z);
			phong_transparency->assignLoc1f("light.ambient", light->getAmbientS());
			phong_transparency->assignLoc1f("light.diffuse", light->getDiffuseS());
			phong_transparency->assignLoc1f("light.specular", light->getSpecularS());
			phong_transparency->assignLoc3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
			phong_transparency->assignLoc3f("material.ambient", 0.25f, 0.25f, 0.25f);
			phong_transparency->assignLoc3f("material.diffuse", 0.4f, 0.4f, 0.4f);
			phong_transparency->assignLoc3f("material.specular", 0.774597f, 0.774597f, 0.774597f);
			phong_transparency->assignLoc1f("material.shininess", 0.6*128.0f);
			ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(phong_transparency->Program, "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
			glActiveTexture(GL_TEXTURE3);
			sphere_mask->Use();
			glUniform1i(glGetUniformLocation(phong_transparency->Program, "texture1"), 3);
			sphere->Draw(phong_transparency);

			float distance1 = glm::length(cameraPos - glm::vec3(-0.5f, 0.0f, -2.0f));
			float distance2 = glm::length(cameraPos - glm::vec3(-0.5f, 0.0f, 2.0f));
			if (distance1 > distance2)
			{
				transparency->Use();
				ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-.5f, 0.0f, -2.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.0f));
				glUniformMatrix4fv(glGetUniformLocation(transparency->Program, "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
				wall->Draw(transparency);
				ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-.5f, 0.0f, -2.0f)) * glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.0f));
				glUniformMatrix4fv(glGetUniformLocation(transparency->Program, "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
				wall->Draw(transparency);


				ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-.5f, 0.0f, 2.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.0f));
				glUniformMatrix4fv(glGetUniformLocation(transparency->Program, "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
				wall->Draw(transparency);
				ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-.5f, 0.0f, 2.0f)) * glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.0f));
				glUniformMatrix4fv(glGetUniformLocation(transparency->Program, "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
				wall->Draw(transparency);

			}
			else {
				transparency->Use();
				ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-.5f, 0.0f, 2.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.0f));
				glUniformMatrix4fv(glGetUniformLocation(transparency->Program, "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
				wall->Draw(transparency);
				ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-.5f, 0.0f, 2.0f)) * glm::rotate(glm::mat4(1.0f), 90.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.0f));
				glUniformMatrix4fv(glGetUniformLocation(transparency->Program, "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
				wall->Draw(transparency);

				ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-.5f, 0.0f, -2.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.0f));
				glUniformMatrix4fv(glGetUniformLocation(transparency->Program, "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
				wall->Draw(transparency);
				ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-.5f, 0.0f, -2.0f)) * glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.0f));
				glUniformMatrix4fv(glGetUniformLocation(transparency->Program, "model"), 1, GL_FALSE, &ModelMatrix[0][0]);
				wall->Draw(transparency);
			}
			break;
		}

		if (glfwGetTime() - lastTime > timeWall)
		{
			if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && currentRenderMode != WIREFRAME)			currentRenderMode = WIREFRAME;
			if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && currentRenderMode != SOLID)				currentRenderMode = SOLID;
			if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && currentRenderMode != GOURAUD)			currentRenderMode = GOURAUD;
			if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && currentRenderMode != PHONG)				currentRenderMode = PHONG;
			if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && currentRenderMode != TEXTURE)			currentRenderMode = TEXTURE;
			if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS && currentRenderMode != NORMAL_TEXTURE)	currentRenderMode = NORMAL_TEXTURE;
			if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS && currentRenderMode != SKYBOX)			currentRenderMode = SKYBOX;
			if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS && currentRenderMode != REFLECTION)		currentRenderMode = REFLECTION;
			if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS && currentRenderMode != REFRACTION)		currentRenderMode = REFRACTION;
			if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS && currentRenderMode != ALPHA_BLENDING)	currentRenderMode = ALPHA_BLENDING;
			if (lastRenderMode != currentRenderMode)
			{
				lastTime = glfwGetTime();
				if (lastRenderMode == ALPHA_BLENDING)
					glDisable(GL_BLEND);
				lastRenderMode = currentRenderMode;
			}
		}
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}