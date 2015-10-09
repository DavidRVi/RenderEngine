# version 330 core

layout(location = 0) in vec3 vertices;

out vec3 texCoord;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	vec4 pos = projection * view * vec4(vertices, 1.0);
    gl_Position = pos.xyww;
	texCoord = vertices;
}