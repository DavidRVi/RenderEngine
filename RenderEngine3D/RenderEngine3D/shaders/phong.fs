#version 330 core

struct Material 
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 color;
	float ambient;
	float diffuse;
	float specular;
};
// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform vec3 viewPos;

uniform Material material;
uniform Light light;

// Input data interpolated for each fragment
in vec3 Normal;
in vec3 FragPos;
in vec2 UV;

void main(){
	//ambient
	vec3 ambient = light.ambient * material.ambient;
	//diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float delta = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (light.diffuse * light.color) * (delta * material.diffuse);
	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * ((light.color * material.specular) * spec);
	color = (ambient + diffuse + specular);
}
