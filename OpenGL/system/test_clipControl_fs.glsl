#version 450 core
layout(location = 0) out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
	vec4 FragPosLightSpace;
    vec4 ClipPos;
} fs_in;

struct Light {
	vec3 position;
    vec3 color;

	vec3 direction;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;
};

uniform vec3 viewPos;
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

const int lightNum = 2;
uniform Light light[lightNum];

const float PI = 3.14159265359;

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness * roughness;
    float a2     = a * a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return (nom / denom);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return (nom / denom);
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return (ggx1 * ggx2);
}

void main() 
{
    vec3 N = normalize(fs_in.Normal);
    vec3 V = normalize(viewPos - fs_in.FragPos);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    for(int i=0;i<lightNum;i++){
        Light curLight = light[i];

        vec3 L = normalize(curLight.position - fs_in.FragPos);
        vec3 H = normalize(V + L);

        float distance = length(curLight.position - fs_in.FragPos);
        // float attenuation = 1.0/(distance*distance);
        float attenuation = 1.0 / (curLight.constant + curLight.linear * distance + curLight.quadratic * (distance * distance));
        // float attenuation = 1.0;
        vec3 radiance = curLight.color * attenuation;

        vec3 F = FresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; 
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = Lo + ambient;

    color = color / (color + vec3(1.0));

    float depth = fs_in.ClipPos.z;
//    float depth = -0.5;
    if(depth < 0.0){
        depth *= -1.0;
    }
    FragColor = vec4(depth, depth, depth, 1.0f);
}