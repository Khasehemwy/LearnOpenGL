#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;
uniform float exposure;

void main()
{ 
    vec3 hdrColor = texture(screenTexture, TexCoord).rgb;
    vec3 bloomColor = texture(bloomTexture,TexCoord).rgb;
    hdrColor += bloomColor;

    hdrColor = vec3(1.0f) - exp(-hdrColor.rgb * exposure);

    FragColor = vec4(hdrColor,texture(screenTexture, TexCoord).a);
}