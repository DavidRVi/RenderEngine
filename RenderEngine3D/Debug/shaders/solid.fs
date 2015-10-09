#version 330 core

out vec3 color;

in vec3 m_Position;
void main()
{
	color = m_Position;
}