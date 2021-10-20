#version 330 core
out vec4 FragColor;

struct Material {
	float shininess;
	sampler2D texture_diffuse[3];
	sampler2D texture_specular[3];
};

struct Light {
    int type;
    // 1:direct, 2:point, 3:SpotLight

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

vec4 CalculateLight(int i)
{
    vec4 color;

    //点光源
    float distance = length(light.position - fs_in.FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    //环境光
    vec3 ambient = light.ambient * texture(material.texture_diffuse[i], fs_in.TexCoord).rgb;
    //漫反射
    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse[i], fs_in.TexCoord).rgb;
    //镜面反射
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular[i], fs_in.TexCoord).rgb);

    if(light.type == 2){
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
    }

    //阴影
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    color = vec4((ambient + (1.0 - shadow) * (diffuse + specular)) , texture(material.texture_diffuse[i], fs_in.TexCoord).a);

    return color;
}

void main() {
	vec4 color={0,0,0,0};
    for(int i=0;i<3;i++){
        color = CalculateLight(i);
    }

	FragColor = color;
}