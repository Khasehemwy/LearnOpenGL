#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;
uniform float exposure;

void main()
{ 
    FragColor = texture(screenTexture, TexCoord);

    vec3 hdrColor = vec3(1.0f) - exp(-FragColor.rgb * exposure);

    FragColor = vec4(hdrColor,FragColor.a);
}