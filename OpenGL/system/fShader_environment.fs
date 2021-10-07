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

uniform samplerCube cubeMap;

void main() {
	vec4 color={0,0,0,0};
	// 反射
	// vec3 I = normalize(fs_in.FragPos - viewPos);
	// vec3 R = reflect(I, normalize(fs_in.Normal));
	// color = vec4(texture(cubeMap,R).rgb,1.0);

	//折射
	float ratio = 1.0/1.52;
	vec3 I = normalize(fs_in.FragPos - viewPos);
	vec3 R = refract(I, normalize(fs_in.Normal),ratio);
	color = vec4(texture(cubeMap,R).rgb,1.0);

	FragColor = color;
}