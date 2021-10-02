#version 330 core
out vec4 FragColor;

struct Material {
	float shininess;
	sampler2D texture_diffuse[3];
	sampler2D texture_specular[3];
};

struct Light {
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;
};

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
	vec4 FragPosLightSpace;
} fs_in;

uniform Material material;
uniform Light light;
uniform sampler2D shadowMap;

uniform vec3 viewPos;

void main() {
	vec4 color={0,0,0,0};
    for(int i=0;i<3;i++){
        color = vec4(texture(material.texture_diffuse[i], fs_in.TexCoord));
    }

	FragColor = color;
}