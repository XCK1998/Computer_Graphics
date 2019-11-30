#version 330 core
in vec3 Normal;
in vec3 FragPos;
out vec4 color;

uniform vec3 ViewPos;

void main()
{
//ambient
color=vec4(1.0f,0.0f,0.0f,1.0f);
vec3 antient = 0.8f*color.rgb;
}