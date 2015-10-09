#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normal;
out vec3 Normal;
out vec3 FragPos;
// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 M;

void main()
{
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
	Normal = mat3(transpose(inverse(M)))*normal;
	//Fragment Position in world space
	FragPos = (M * vec4(vertexPosition_modelspace,1)).xyz;
}


