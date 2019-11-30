#version 330 core
in vec2 Texcoord;
out vec4 color;

uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;

void main()
{
//color=vec4(1.0f,0.0f,0.0f,1.0f);
color=texture(ourTexture0,Texcoord);
}