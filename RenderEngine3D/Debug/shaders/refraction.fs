#version 330 core

in vec3 Normal;
in vec3 Position_worldspace;

uniform vec3 cameraPos;
uniform samplerCube skybox;

out vec4 color;

void main()
{
	float ratio = 1.0/1.52;
	vec3 viewDir = normalize(Position_worldspace - cameraPos);
	vec3 refractDir = refract(viewDir, normalize(Normal), ratio);
	color = texture(skybox, refractDir);
}
