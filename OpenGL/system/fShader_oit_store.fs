#version 420 core
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

layout (early_fragment_tests) in;

uniform Material material;
uniform Light light;
layout (binding = 2) uniform sampler2D shadowMap;

uniform vec3 viewPos;

layout (binding = 0,offset = 0)uniform atomic_uint index_counter;
layout (binding = 3,r32ui) uniform uimage2D head_pointer_image;
layout (binding = 4,rgba32ui) uniform writeonly uimageBuffer list_buffer;


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
	
	uint index = atomicCounterIncrement(index_counter);
	uint old_head = imageAtomicExchange(head_pointer_image,ivec2(gl_FragCoord.xy),index);
	uvec4 item;
	item.x=old_head;
	item.y=packUnorm4x8(color);
	item.z=floatBitsToUint(gl_FragCoord.z);
	item.w=0;

	imageStore(list_buffer, int(index), item);

	FragColor=color;
}