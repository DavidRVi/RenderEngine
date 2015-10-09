#version 330 core
in vec3 Normal;
in vec3 Position_worldspace;
out vec4 color;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{             
    vec3 viewDir = normalize(Position_worldspace - cameraPos);
    vec3 reflectDir = reflect(viewDir, normalize(Normal));
    color = texture(skybox, reflectDir);
}
