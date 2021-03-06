#version 330 core
out vec4 FragColor;

struct Material {
	float shininess;
	sampler2D texture_diffuse0;
	sampler2D texture_specular0;
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

uniform vec3 viewPos;


void main() {
    //点光源
    float distance = length(light.position - fs_in.FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	//环境光
	vec3 ambient = light.ambient * texture(material.texture_diffuse0, fs_in.TexCoord).rgb;
    ambient *= attenuation;
	//漫反射
	vec3 norm = normalize(fs_in.Normal);
	vec3 lightDir = normalize(light.position - fs_in.FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse0, fs_in.TexCoord).rgb;
    diffuse *= attenuation;
	//镜面反射
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular0, fs_in.TexCoord).rgb);
    specular *= attenuation;

	//阴影
	vec4 color = vec4((ambient + diffuse + specular) , texture(material.texture_diffuse0, fs_in.TexCoord).a);

	FragColor = color;
}