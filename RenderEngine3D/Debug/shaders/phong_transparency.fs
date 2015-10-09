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
out vec4 color;

// Values that stay constant for the whole mesh.
uniform vec3 viewPos;

uniform Material material;
uniform Light light;

// Input data interpolated for each fragment
in vec3 Normal;
in vec3 FragPos;
in vec2 UV;

uniform sampler2D texture1;

void main(){
	//ambient
	vec4 texColor = texture(texture1,UV);
	
	if(texColor.a < 0.1)
		discard;
	else {
		vec4 ambient = light.ambient * texColor;
		//diffuse
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(light.position - FragPos);
		float delta = max(dot(norm, lightDir), 0.0);
		vec4 diffuse = (light.diffuse * vec4(light.color, 1.0)) * (delta * texColor);
		//specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir,norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec4 specular = light.specular * ((vec4(light.color, 1.0) * texColor) * spec);
		//color = FragPos;
		color = (ambient + diffuse + specular);
	}
}
