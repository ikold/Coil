#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out vec3 outPosition;
out vec4 vColor;

void main()
{
	outPosition = position;
	vColor = color;
	gl_Position = vec4(position, 1.0);
}