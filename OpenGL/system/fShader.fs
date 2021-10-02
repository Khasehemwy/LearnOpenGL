#version 330 core
out vec4 FragColor;

struct Material {
	float shininess;
	sampler2D texture_diffuse0;
	sampler2D texture_specular0;
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_diffuse2;
	sampler2D texture_specular2;
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

float ShadowCalculation(vec4 fragPosLightSpace) {
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float shadow = 0;
	if(currentDepth > 1.0) {
		shadow = 0.0;
	} 
	else {
		float bias = 0.0010;
		vec2 texelsize = 1.0 / textureSize(shadowMap, 0);
		float cnt=0;
		for(int x = -1; x <= 1; x++) {
			for(int y = -1; y <= 1; y++) {
				float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelsize).r;
				shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
				cnt++;
			}
		}
		shadow /= cnt;
	}
	return shadow;
}

void main() {
	// if(texture(material.texture_diffuse0, fs_in.TexCoord).a < 1.0f){discard;}

	//环境光
	vec3 ambient = light.ambient * texture(material.texture_diffuse0, fs_in.TexCoord).rgb;
	//漫反射
	vec3 norm = normalize(fs_in.Normal);
	vec3 lightDir = normalize(light.position - fs_in.FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse0, fs_in.TexCoord).rgb;
	//镜面反射
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.texture_specular0, fs_in.TexCoord).rgb);

	//阴影
	float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
	vec4 color = vec4((ambient + (1.0 - shadow) * (diffuse + specular)) , texture(material.texture_diffuse0, fs_in.TexCoord).a);

	FragColor = color;
}