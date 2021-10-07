#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out VS_OUT {
        vec3 FragPos;
        vec3 Normal;
        vec2 TexCoord;
        mat4 view;
} vs_out;

uniform mat4 model;
uniform mat4 view;

void main() {
        gl_Position = view * model * vec4(aPos, 1.0); 
        vs_out.TexCoord = aTexCoord;
        mat3 normalMatrix = mat3(transpose(inverse(view * model)));
        vs_out.Normal = normalize(vec3(vec4(normalMatrix * aNormal, 0.0)));
        // vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
        vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
        vs_out.view = view;
}