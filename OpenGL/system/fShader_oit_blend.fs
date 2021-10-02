#version 420 core
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
layout (binding = 2) uniform sampler2D shadowMap;
uniform vec3 viewPos;

layout (binding = 3,r32ui) uniform uimage2D head_pointer_image;
layout (binding = 4,rgba32ui) uniform uimageBuffer list_buffer;
#define MAX_FRAGMENTS 16
uvec4 fragments[MAX_FRAGMENTS];

int Build_Local_Fragment_List()
{
	uint current;
	int frag_count=0;
	current = uint(imageLoad(head_pointer_image, ivec2(gl_FragCoord).xy).x);

	uvec4 item;
	while(current!=0 && frag_count < MAX_FRAGMENTS)
	{
		item = uvec4(imageLoad(list_buffer,int(current)));
		current=item.x;
		fragments[frag_count] = item;
		frag_count++;
	}
	return frag_count;
}

void Sort_Fragment_List(int frag_count)
{
	for(int i=0;i<frag_count;i++){
		float depth_i = uintBitsToFloat(fragments[i].z);
		for(int j=i+1;j<frag_count;j++){
			float depth_j = uintBitsToFloat(fragments[j].z);
			if(depth_i<depth_j){
				uvec4 temp= fragments[i];
				fragments[i]=fragments[j];
				fragments[j]=temp;
			}
		}
	}
}

vec4 blend(vec4 current_color,vec4 new_color){
	return mix(current_color,new_color,new_color.a);
}
vec4 Calculate_Final_Color(int frag_count)
{
	vec4 color = vec4(0.0);

	for(int i=0;i<frag_count;i++){
		vec4 frag_color = unpackUnorm4x8(fragments[i].y);
		// if(frag_color.a<1.0)discard;
		color=blend(color,frag_color);
	}
	return color;
}

void main() {
	int frag_count;
	frag_count = Build_Local_Fragment_List();

	Sort_Fragment_List(frag_count);

	vec4 color = Calculate_Final_Color(frag_count);

	FragColor = color;
}