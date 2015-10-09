#version 330 core
layout(location = 0) in vec3 vertexPosition;

uniform mat4 model;
uniform mat4 MVP;

out vec3 m_Position;

void main()
{
	gl_Position = MVP * vec4(vertexPosition,1);
	m_Position = (model* vec4(vertexPosition,1)).xyz;
}