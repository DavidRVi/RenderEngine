#version 330 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;

uniform mat4 MVP;

out vec3 vertex_color;

void main()
{
	gl_Position = MVP * vec4(vertexPosition,1);
	vertex_color = vertexColor;
}

