#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

in VS_OUT {
        vec3 FragPos;
        vec3 Normal;
        vec2 TexCoord;
        mat4 view;
} gs_in[];

uniform mat4 projection;

const float MAGNITUDE = 0.04;
void GenerateLine(vec4 pos, vec3 normal)
{
    gl_Position = projection * pos;
    EmitVertex();
    gl_Position = projection * (pos + vec4(normal, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main() 
{
    vec4 pos = gl_in[0].gl_Position+gl_in[1].gl_Position+gl_in[2].gl_Position;
    pos=pos/3.0f;
    vec3 normal = gs_in[0].Normal + gs_in[1].Normal + gs_in[2].Normal;
    normal = normal/3.0f;

    GenerateLine(pos,normal); 
}