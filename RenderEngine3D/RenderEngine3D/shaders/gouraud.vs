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

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 normal;

out vec3 inColor;

// Values that stay constant for the whole mesh.
uniform mat4 M;
uniform mat4 MVP;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
	vec3 Normal = mat3(transpose(inverse(M)))*normal;
	//Vertex Position in world space
	vec3 vertexPos = (M * vec4(vertexPosition_modelspace,1)).xyz;
	
	//ambient
	vec3 ambient = light.ambient * material.ambient;
	
	//diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - vertexPos);
	float delta = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (delta * material.diffuse);
	
	//specular
	vec3 viewDir = normalize(viewPos - vertexPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (material.specular * spec);
	
	inColor = ambient + diffuse + specular;
}

