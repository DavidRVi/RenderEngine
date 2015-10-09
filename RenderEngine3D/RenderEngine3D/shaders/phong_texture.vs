#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uvCoords;
out vec3 Normal;
out vec3 FragPos;
out vec2 UV;
uniform mat4 MVP;
uniform mat4 M;

void main(){
	gl_Position =  MVP* vec4(vertexPosition_modelspace,1);
	Normal = mat3(transpose(inverse(M)))*normal;
	UV = uvCoords;
	//Fragment Position in world space
	FragPos = (M * vec4(vertexPosition_modelspace,1)).xyz;
}

