#version 330 core

layout(location = 0) out vec4 color;

in vec3 vPosition;
in vec4 vColor;

void main()
{
	color = vec4(vPosition * 0.5 + 0.5, 1.0);
	color = vColor;
}