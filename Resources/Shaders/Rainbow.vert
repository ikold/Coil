#version 330 core

layout(location = 0) in vec3 aPosition;

uniform mat4 uViewProjection;

out vec3 vPosition;

void main()
{
	vPosition = aPosition;
	gl_Position = uViewProjection * vec4(aPosition, 1.0);
}