#version 330 core

struct Light
{
	vec3 position;
	vec3 color;
	float ambient;
	float diffuse;
	float specular;
};
out vec3 color;
uniform vec3 viewPos;
uniform Light light;

uniform sampler2D diffTexture;
uniform sampler2D normalTexture;
// Input data interpolated for each fragment
in vec3 Normal;
in vec3 FragPos;
in vec2 UV;
void main(){
	//ambient
	vec3 objectColor = vec3(texture(diffTexture, UV));
	vec3 ambient = light.ambient * objectColor;
	//diffuse
	vec3 norm = normalize(Normal * vec3(texture(normalTexture,UV)));
	vec3 lightDir = normalize(light.position - FragPos);
	float delta = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (delta * objectColor);
	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float shininess = 32;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = light.specular * (light.color * spec);
	color = ambient + diffuse + specular;
}


