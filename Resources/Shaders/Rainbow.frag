#version 330 core

layout(location = 0) out vec4 color;

in vec3 outPosition;

void main()
{
	color = vec4(outPosition * 0.5 + 0.5, 1.0);
}