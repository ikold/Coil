#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;

uniform mat4 uViewProjection;

out vec3 vPosition;
out vec4 vColor;

void main()
{
	vPosition = aPosition;
	vColor = aColor;
	gl_Position = uViewProjection * vec4(aPosition, 1.0);
}