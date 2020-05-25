#type vertex
#version 330 core

layout(location = 0) in vec3 aPosition;

uniform mat4 uViewProjection;
uniform mat4 uTransform;

out vec3 vPosition;

void main()
{
	vPosition = aPosition;
	gl_Position = uViewProjection * uTransform * vec4(aPosition, 1.0);
}


#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 vPosition;

void main()
{
	color = vec4(vPosition * 0.5 + 0.5, 1.0);
}