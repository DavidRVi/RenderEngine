#version 330 core

uniform sampler2D texture1;
in vec2 texCoord;
out vec4 color;
void main() {
    vec4 texColor = texture(texture1, texCoord);
	if(texColor.a < 0.1)
		discard;
	color = texColor;
}
