#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 Normal;
out vec3 Position_worldspace;

uniform mat4 model;
uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(position, 1.0f);
    Normal = mat3(transpose(inverse(model))) * normal;
    Position_worldspace = vec3(model * vec4(position, 1.0f));
}  
