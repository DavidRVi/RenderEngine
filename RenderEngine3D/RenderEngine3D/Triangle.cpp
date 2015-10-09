#include "Triangle.h"

Triangle::Triangle()
{
	GLfloat vertices[] = {
		// Posiciones       // Colores
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // Derecha abajo
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // Izquierda abajo
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // Arriba
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Enlazar el VAO primero, luego el VBO y llenar el VBO con los datos
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Atributo 0 = Posición
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Atributo 1 = Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Desenlazar VAO
}

void Triangle::Draw(Shader* shader)
{
	shader->Use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}