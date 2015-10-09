#version 330 core

struct Material 
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	float ambientReflection;
	float diffuseReflection;
	float specularReflection;
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

uniform samplerCube skybox;

void main(){

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(FragPos - viewPos);
	vec3 lightDir = normalize(light.position - FragPos);
	vec3 halfVector = normalize((-lightDir + viewDir) / length(-lightDir + viewDir));
    vec3 reflectDir = reflect(halfVector, normalize(Normal));
	vec4 environmentMap = texture(skybox, reflectDir);
	//ambient
	vec3 ambient = (light.ambient * material.ambient) + (environmentMap.xyz * material.ambientReflection);
	//diffuse
	
	
	float delta = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = (light.diffuse * light.color) * (delta * material.diffuse) + delta *(environmentMap.xyz * material.diffuseReflection);
	//specular
	viewDir = normalize(viewPos - FragPos);
	reflectDir = reflect(-lightDir,norm);
	float shininess = 32;
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	//vec3 specular = light.specular * ((light.color * material.specular) * spec);
	vec3 specular = light.specular * (light.color  * spec) + spec *(environmentMap.xyz * material.specularReflection);
	//color = FragPos;
	color = (ambient + diffuse + specular);
}
