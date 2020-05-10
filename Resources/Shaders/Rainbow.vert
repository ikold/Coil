#version 330 core

layout(location = 0) in vec3 position;

out vec3 outPosition;

void main()
{
	outPosition = position;
	gl_Position = vec4(position, 1.0);
}