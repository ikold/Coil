#version 330 core

layout(location = 0) out vec4 color;

in vec2 vTextureCoordinates;

void main()
{
	color = vec4(vTextureCoordinates, 0, 1.0);
}