#version 330 core
layout(location=0) in vec3 position;
layout(location=1) in vec2 texcoord;


out vec2 Texcoord;

void main()
{
gl_Position=vec4(position.x,position.y,position.z,1.0f);
Texcoord=vec2(texcoord.x,1-texcoord.y);
}